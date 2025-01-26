#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <string>

#include "./../lib/library.h"

extern "C" {
    bool function3(const char* str);
}

bool exit_flag = false;

void process_data(const std::string& s) {
    if (function3(s.c_str())) {
        std::cout << "Data received!" << std::endl;
    }
    else {
        std::cerr << "Error: received invalid data: ( " << s << " )" <<  std::endl;
    }
}

void server() {
    while(!exit_flag) {
        std::string s;
        if (!std::getline(std::cin, s)) {
            exit_flag = true;
            break;
        }

        if (s == "Q") {
            exit_flag = true;
            break;
        }

        process_data(s);
    }
}

int main() {
    std::thread s1(server);

    s1.join();

    return 0;
}