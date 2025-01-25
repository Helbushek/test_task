#include <string>
#include <cstring>
#include <algorithm>
#include "library.h"

void function1(char* str) {
    std::string s(str);
    std::sort(s.rbegin(), s.rend());
    for (int i=0; i<s.size(); i += 2) {
        if (i<s.size()) {
            s.replace(i, i, "KB");
        }
    }
    strcpy_s(str, s.size()+1, s.c_str());
}

int function2(const char* str) {
    int sum = 0;
    for (; *str; *str++) {
        if (isdigit(*str)) {
            sum += *str - '0';
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