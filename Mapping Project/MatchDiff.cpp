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
    int count = 0;
    auto len = a.length();
    auto c_a = a.data();
    auto c_b = b.data();
    for (int i = 0; i < len; i++) {
        if (std::toupper(c_a[i]) != std::toupper(c_b[i])) {
            count++;
        }
    }
    if (count > diff) {
        return false;
    } else {
        return true;
    }
}