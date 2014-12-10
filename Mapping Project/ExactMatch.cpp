//
//  ExactMatch.cpp
//  Mapping Project
//
//  Created by Peter Anderson on 12/6/14.
//  Copyright (c) 2014 VanWinkle Enterprises. All rights reserved.
//

#include "ExactMatch.h"

bool ExactMatch::equal(std::string a, std::string b) {
    return icompare(a, b);
}

bool ExactMatch::icompare_pred(unsigned char a, unsigned char b) {
    return std::tolower(a) == std::tolower(b);
}

bool ExactMatch::icompare(std::string const& a, std::string const& b) {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), icompare_pred);
}
