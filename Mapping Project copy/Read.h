//
//  Read.h
//  Mapping Project
//
//  Created by Peter Anderson on 12/8/14.
//  Copyright (c) 2014 VanWinkle Enterprises. All rights reserved.
//

#ifndef __Mapping_Project__Read__
#define __Mapping_Project__Read__
#include <string>
#include <set>

class Read {
public:
    Read(std::string name, std::string reference_name,
         std::string sequence);
    ~Read() {};
    auto toString() const -> std::set<std::string>;
    void setPositions(const std::set<int>& positions);
    
    const std::string name;
    const std::string reference_name;
    const std::string sequence;
    
private:
    int flag;
    std::set<int> positions; // 1-based
    int mapping_quality;
    std::string cigar;
    std::string next_read;
    int next_pos;
    int template_len;
    std::string phred;
};

#endif /* defined(__Mapping_Project__Read__) */
