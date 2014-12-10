//
//  StringCompare.h
//  Mapping Project
//
//  Created by Peter Anderson on 12/6/14.
//  Copyright (c) 2014 VanWinkle Enterprises. All rights reserved.
//

#ifndef __Mapping_Project__StringCompare__
#define __Mapping_Project__StringCompare__

#include <stdio.h>
#include <string>

class StringCompare {
public:
    StringCompare() {};
    ~StringCompare() {};
    virtual bool equal(std::string a, std::string b) = 0;
};

#endif /* defined(__Mapping_Project__StringCompare__) */
