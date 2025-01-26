#include <string>
#include <cstring>
#include <algorithm>
#include "library.h"

void function1(char* str) {
    std::string s(str);
    std::sort(s.rbegin(), s.rend());
    for (int i=1; i<s.size(); i += 2) {
        if (i<s.size()) { 
            s.replace(i++, 1, "KB");
        }
    }
    strncpy(str, s.c_str(), s.size() + 1);
}

int function2(const char* str) {
    int sum = 0;
    for (; *str; *str++) {
        if (isdigit(*str)) {
            sum += (*str - '0');
        }
    }
    return sum;
}   

bool function3(const char* str) {
    size_t len = strlen(str);
    if (len > 2 && len % 32 == 0) {
        return true;
    }
    return false;
} 