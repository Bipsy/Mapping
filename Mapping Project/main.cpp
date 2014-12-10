//
//  main.cpp
//  Mapping Project
//
//  Created by Peter Anderson on 12/6/14.
//  Copyright (c) 2014 VanWinkle Enterprises. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <memory>
#include <set>
#include <string>
#include <locale>
#include "StringCompare.h"
#include "ExactMatch.h"
#include "MatchDiff.h"
#include "Read.h"

using namespace std;

auto produceKmers(const vector<pair<string, int>>& kmers,
                  string strand,
                  int k, int gap=1) -> vector<pair<string, int>> {
    auto res = kmers;
    auto len = strand.length();
    for (int i = 0; i + k <= len; i+=gap) {
        auto kmer = strand.substr(i, k);
        res.push_back(pair<string, int>(kmer, i));
    }
    return res;
}

auto parseReferenceFile(const char* file_name,
                        const char* program_name) -> tuple<string, string> {
    ifstream in_file(file_name);
    if (!in_file.good()) {
        std::cerr << "Usage: " << program_name;
        std::cerr << " <reference_genome> <read_sequences>" << endl;
        exit(1);
    }
    
    string line, name, content;
    while (std::getline(in_file, line).good()) {
        if (line.empty() || line[0] == '>') {
            if (!line.empty()) {
                name = line.substr(1);
            }
            content.clear();
        } else if (!name.empty()) {
            content += line;
        }
    }
    
    return tuple<string, string>(content, name);
}

auto parseReadFile(const char* file_name, const char* program_name,
                   const string& reference_name,
                   vector<Read>& reads) -> vector<Read> {
    ifstream in_file(file_name);
    if (!in_file.good()) {
        std::cerr << "Usage: " << program_name;
        std::cerr << " <reference_genome> <read_sequences>" << endl;
        exit(1);
    }
    
    string content, line, name;
    while (std::getline(in_file, line).good()) {
        if(line.empty() || line[0] == '>') {
            if(!name.empty()) { // Print out what we read from the last entry
                Read read(name, reference_name, content);
                reads.push_back(read);
                name.clear();
            }
            if(!line.empty()) {
                name = line.substr(1);
            }
            content.clear();
        } else if(!name.empty()) {
            if(line.find(' ') != std::string::npos){ // Invalid sequence--no spaces allowed
                name.clear();
                content.clear();
            } else {
                content += line;
            }
        }
    }
    
    if(!name.empty()) { // Print out what we read from the last entry
        Read read(name, reference_name, content);
    }
    
    return reads;
}

auto insertKmers(vector<pair<string, int>>& kmers,
                 unordered_map<string, vector<int>>& table)
                 -> unordered_map<string, vector<int>> {
    for (auto it = kmers.begin(); it != kmers.end(); ++it) {
        auto it_t = table.find(it->first);
        if (it_t == table.end()) {
            auto indices = {it->second};
            table[it->first] = indices;
        } else {
            table[it->first].push_back(it->second);
        }
    }
    return table;
}

auto retrieveIndices(string read, const unordered_map<string, vector<int>>& table) -> vector<int> {
    auto it = table.find(read);
    if (it != table.end()) {
        return it->second;
    } else {
        return vector<int>();
    }
}

auto mappedLocations(Read& read, const string& reference_genome, int k,
                   const unordered_map<string, vector<int>>& table,
                   unique_ptr<StringCompare>& comparator) {
    set<int> locations;
    auto len = read.sequence.length();
    auto read_segments = produceKmers(vector<pair<string, int>>(), read.sequence, k, 10);
    for (auto segment = read_segments.begin(); segment != read_segments.end(); ++segment) {
        auto indices = retrieveIndices(segment->first, table);
        for (auto index = indices.begin(); index != indices.end(); ++index) {
            if ((*index) - segment->second >= 0) {
                auto dna_fragment = reference_genome.substr((*index) - segment->second, len);
                if (comparator->equal(dna_fragment, read.sequence)) {
                    locations.insert((*index) - segment->second);
                }
            }
        }
    }
    read.setPositions(locations);
}

auto printLocations(const vector<Read>& mapped_locations) -> void {
    for (auto mapping = mapped_locations.begin(); mapping != mapped_locations.end(); ++mapping) {
        auto read_strings = mapping->toString();
        for (auto read_string : read_strings) {
            cout << read_string << endl;
        }
    }
}

int main(int argc, const char * argv[]) {
    // Global Data Structures
    vector<Read> reads;
    int k = 50; // This value needs to be dynamically determined
    vector<pair<string, int>> kmers;
    unordered_map<string, vector<int>> hash_table;
    //-----------------------------------------------
    
    // Read Input---------------------------------------
    string reference_genome, reference_name;
    tie(reference_genome, reference_name) = parseReferenceFile(argv[1], argv[0]);
    reads = parseReadFile(argv[2], argv[0], reference_name, reads);
    //--------------------------------------------------
    
    
    kmers = produceKmers(kmers, reference_genome, k, 1);
    hash_table = insertKmers(kmers, hash_table);
    
    //Perform Mapping---------------------------------------
    unique_ptr<StringCompare> comparator = make_unique<MatchDiff>(1);
    for (auto it = reads.begin(); it != reads.end(); ++it) {
        mappedLocations(*it, reference_genome, k, hash_table, comparator);
    }
    //------------------------------------------------------
    
    printLocations(reads);
    return 0;
}
