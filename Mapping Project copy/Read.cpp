//
//  Read.cpp
//  Mapping Project
//
//  Created by Peter Anderson on 12/8/14.
//  Copyright (c) 2014 VanWinkle Enterprises. All rights reserved.
//

#include "Read.h"
#include <string>
#include <sstream>
#include <set>

Read::Read(std::string name, std::string reference_name,
           std::string sequence)
            : name(name), sequence(sequence), reference_name(reference_name) {
    mapping_quality = 255;
    flag = 0;
    cigar = "9M";
    next_read = "*";
    next_pos = 0;
    template_len = 0;
    phred = "*";
}

auto Read::toString() const -> std::set<std::string> {
    std::set<std::string> strings;
    for (auto pos = positions.begin(); pos != positions.end(); ++pos) {
        std::stringstream ss;
        ss << name << "\t" << flag << "\t" << reference_name << "\t";
        ss << *pos << "\t" << mapping_quality << "\t" << cigar << "\t";
        ss << next_read << "\t" << next_pos << "\t" << template_len << "\t";
        ss << sequence << "\t" << phred;
        strings.insert(ss.str());
    }
    return strings;
}

auto Read::setPositions(const std::set<int>& positions) -> void {
    this->positions = positions;
}