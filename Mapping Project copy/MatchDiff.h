//
//  MatchDiff.h
//  Mapping Project
//
//  Created by Peter Anderson on 12/8/14.
//  Copyright (c) 2014 VanWinkle Enterprises. All rights reserved.
//

#ifndef __Mapping_Project__MatchDiff__
#define __Mapping_Project__MatchDiff__

#include "StringCompare.h"

class MatchDiff : public StringCompare {
public:
    MatchDiff();
    MatchDiff(int diff);
    bool equal(std::string a, std::string b);
private:
    int diff;
};

#endif /* defined(__Mapping_Project__MatchDiff__) */
