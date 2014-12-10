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

auto convertToUpperCase(const string& strand) -> string {
    locale loc;
    string res = strand;
    size_t len = strand.length();
    for (int i = 0; i < len; i++) {
        res[i] = toupper(strand[i], loc);
    }
    return res;
}

auto removeWhiteSpace(const string& strand) -> string {
    string res = strand;
    for (auto it = res.begin(); it != res.end(); ++it) {
        if (isspace(*it)) {
            res.erase(it);
            --it;
        }
    }
    return res;
}

auto produceKmers(const vector<pair<string, int>>& kmers,
                  string strand,
                  int k) -> vector<pair<string, int>> {
    auto res = kmers;
    auto len = strand.length();
    for (int i = 0; i + k <= len; ++i) {
        auto kmer = strand.substr(i, k);
        kmer = convertToUpperCase(kmer);
        res.push_back(pair<string, int>(kmer, i));
    }
    return res;
}

auto parseReferenceFile(const char* file_name) -> tuple<string, string> {
    ifstream in_file(file_name);
    string content;
    content.assign((istreambuf_iterator<char>(in_file)), istreambuf_iterator<char>());
    size_t new_line = content.find_first_of('\n');
    auto reference_name = content.substr(1, new_line);
    content.erase(0, new_line+1);
    in_file.close();
    return tuple<string, string>(content, reference_name);
}

auto parseReadFile(const char* file_name, const string& reference_name,
                   vector<Read>& reads) -> vector<Read> {
    ifstream in_file(file_name);
    string content;
    content.assign((istreambuf_iterator<char>(in_file)), (istreambuf_iterator<char>()));
    content = convertToUpperCase(content);
    while (!content.empty()) {
        auto arr_pos = content.find_first_of('>');
        auto new_line = content.find_first_of('\n', arr_pos);
        auto read_name = content.substr(arr_pos+1, new_line - (arr_pos+1));
        content.erase(0, new_line+1);
        arr_pos = content.find_first_of('>');
        if (arr_pos == string::npos) {
            content = removeWhiteSpace(content);
            Read read(read_name, reference_name, content);
            reads.push_back(read);
            break;
        } else {
            auto read_sequence = content.substr(0, arr_pos);
            read_sequence = removeWhiteSpace(read_sequence);
            Read read(read_name, reference_name, read_sequence);
            reads.push_back(read);
            content.erase(0, arr_pos);
        }
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
    auto read_segments = produceKmers(vector<pair<string, int>>(), read.sequence, k);
    for (auto segment = read_segments.begin(); segment != read_segments.end(); ++segment) {
        auto indices = retrieveIndices(segment->first, table);
        for (auto index = indices.begin(); index != indices.end(); ++index) {
            if ((*index) - segment->second >= 0) {
                auto dna_fragment = reference_genome.substr((*index) - segment->second, len);
                dna_fragment = convertToUpperCase(dna_fragment);
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
    tie(reference_genome, reference_name) = parseReferenceFile(argv[1]);
    reference_genome = removeWhiteSpace(reference_genome);
    reads = parseReadFile(argv[2], reference_name, reads);
    //--------------------------------------------------
    
    
    kmers = produceKmers(kmers, reference_genome, k);
    hash_table = insertKmers(kmers, hash_table);
    
    //Perform Mapping---------------------------------------
    unique_ptr<StringCompare> comparator = make_unique<ExactMatch>();
    for (auto it = reads.begin(); it != reads.end(); ++it) {
        mappedLocations(*it, reference_genome, k, hash_table, comparator);
    }
    //------------------------------------------------------
    
    printLocations(reads);
    return 0;
}
