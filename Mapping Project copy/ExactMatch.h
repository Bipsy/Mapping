
//
//  ExactMatch.h
//  Mapping Project
//
//  Created by Peter Anderson on 12/6/14.
//  Copyright (c) 2014 VanWinkle Enterprises. All rights reserved.
//

#ifndef __Mapping_Project__ExactMatch__
#define __Mapping_Project__ExactMatch__

#include <stdio.h>
#include <string>
#include "StringCompare.h"

class ExactMatch : public StringCompare {
public:
    virtual bool equal(std::string a, std::string b);
};

#endif /* defined(__Mapping_Project__ExactMatch__) */
