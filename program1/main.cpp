#include <thread>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <queue>
#include <algorithm>
#include <locale>
#include <clocale>
#include <cstring>

#include "./../lib/library.h"

extern "C" {
    void function1(char* str);
    int function2(const char* str);
    bool function3(const char* str);
}

std::mutex m;
std::condition_variable cv;
std::queue<std::string> buffer;
bool exit_flag = false;

void receiver() {
    while (!exit_flag) {
        std::string input;
        std::getline(std::cin, input);

        if (input == "Q") {
            exit_flag = true;
            cv.notify_one();
            break;
        }

        // check for length constraint 
        if (input.length() > 64) {
            std::cerr << "Error: Max length is 64 symbols\n";
            continue;
        }
        //check for only numbers constraint
        bool valid = std::all_of(input.begin(), input.end(), [](char c) { return std::isdigit(c); });

        if (!valid) {
            std::cerr << "Error: Only digits allowed\n";
            continue;
        }
        // Give input to function 1 of library
        char* cstr = new char[int(input.size()*1.5) + 2];
        strncpy(cstr, input.c_str(), input.size()+1);
        function1(cstr);
        std::string processed_input(cstr);
        delete[] cstr;
        // push data to buffer
        {
            std::lock_guard<std::mutex> lock(m);
            buffer.push(processed_input);
            cv.notify_one();
        }
    }
}

void sender() {
    while (true) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [] { return !buffer.empty() || exit_flag; });
        if (exit_flag && buffer.empty()) break;
        if (!buffer.empty()) {
            std::string data = buffer.front();
            buffer.pop();
            std::cout << data;
            // pass data to function 2 of library
            int sum = function2(data.c_str());
            // pass result to program 2
            std::cout << "Data: " << data << " | sum = " << sum << std::endl;
        }
    }
}

int main() {
    std::locale::global(std::locale("C"));

    std::thread t1(receiver);
    std::thread t2(sender);

    t1.join();
    t2.join();

    return 0;
}

