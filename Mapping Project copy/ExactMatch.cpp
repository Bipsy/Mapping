//
//  ExactMatch.cpp
//  Mapping Project
//
//  Created by Peter Anderson on 12/6/14.
//  Copyright (c) 2014 VanWinkle Enterprises. All rights reserved.
//

#include "ExactMatch.h"

bool ExactMatch::equal(std::string a, std::string b) {
    if (a.compare(b) == 0) {
        return true;
    } else {
        return false;
    }
}
