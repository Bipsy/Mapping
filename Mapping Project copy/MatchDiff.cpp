//
//  MatchDiff.cpp
//  Mapping Project
//
//  Created by Peter Anderson on 12/8/14.
//  Copyright (c) 2014 VanWinkle Enterprises. All rights reserved.
//

#include "MatchDiff.h"

MatchDiff::MatchDiff(int diff) {
    this->diff = diff;
}

MatchDiff::MatchDiff() {
    diff = 0;
}

bool MatchDiff::equal(std::string a, std::string b) {
    int count = 0, index = 0;
    while (count <= diff) {
        if (index < a.length()) {
            if (index < b.length()) {
                if (a[index] != b[index]) {
                    count++;
                }
            } else {
                count++;
            }
        } else {
            if (index < b.length()) {
                count++;
            } else {
                break;
            }
        }
        index++;
    }
    if (count <= diff) {
        return true;
    } else {
        return false;
    }
}