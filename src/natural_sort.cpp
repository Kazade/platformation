/***********************************************************************************
*
*  This program is free software; you can redistribute it and/or modify 
*  it under the terms of the GNU Lesser General Public License as published 
*  by the Free Software Foundation; either version 3 of the License, or (at 
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but 
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public 
*  License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License 
*  along with this program; if not, see <http://www.gnu.org/copyleft/lesser.html>.
*
**********************************************************************************/


#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

#include "natural_sort.h"

std::vector<std::string> get_blocks(const std::string& s) {
    std::vector<std::string> blocks;

    enum Type {
        DIGIT,
        ALPHA
    };

    Type current = isdigit(s[0]) ? DIGIT : ALPHA;

    std::vector<char> block;

    for(unsigned i = 0; i < s.size(); ++i) {
        char x = s[i];
        Type x_type = isdigit(x) ? DIGIT : ALPHA;

        if(x_type == current) {
            block.push_back(x);
        } else {
            blocks.push_back(std::string(block.begin(), block.end()));
            block.clear();
            block.push_back(x);
            current = x_type;
        }
    }

    return blocks;
}


bool natural_sort_predicate(const std::string& lhs, const std::string& rhs) {
    //Return true only if lhs < rhs

    if(lhs == rhs) {
        return false;
    }

    //Break both strings into blocks of either alphabetical chars, or numeric ones

    std::vector<std::string> lhs_blocks = get_blocks(lhs);
    std::vector<std::string> rhs_blocks = get_blocks(rhs);

    unsigned int i = 0;
    while(i < lhs_blocks.size() && i < rhs_blocks.size()) {
        try {
            //Attempt to do an integer compare, if either fails, do a string compare
            int lhs_int = boost::lexical_cast<int>(lhs_blocks[i]);
            int rhs_int = boost::lexical_cast<int>(rhs_blocks[i]);

            if(lhs_int < rhs_int) {
                return true;
            } else if(lhs_int > rhs_int) {
                return false;
            }

            //Now, if they are equal, continue to the next block
        } catch(boost::bad_lexical_cast& e) {
            if(lhs_blocks[i] < rhs_blocks[i]) {
                return true;
            } else if (lhs_blocks[i] > rhs_blocks[i]) {
                return false;
            }
            //Now, if they are equal, continue to the next block
        }

        ++i;
    }
    return false;
}
