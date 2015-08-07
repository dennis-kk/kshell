#include "util.h"

void split(const std::string& src, const std::string& delim, std::vector<std::string>& result) {
    size_t end   = 0;   
    size_t begin = 0;   
    while (end != std::string::npos) {   
        end = src.find_first_of(delim, begin);
        if (end != std::string::npos) {
            result.push_back(src.substr(begin, src.length() - begin));   
        } else {
            result.push_back(src.substr(begin, end - begin));
        }
        begin = end + 1;   
    }
}
