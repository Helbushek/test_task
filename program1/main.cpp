#include <thread>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <queue>
#include <algorithm>
#include <clocale>
#include <cstring>
#include <atomic>
#include <vector>

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include "./../lib/library.h"

extern "C" {
    void function1(char* str);
    int function2(const char* str);
}

std::mutex m;
std::condition_variable cv;
std::queue<std::string> buffer;
std::atomic<bool> exit_flag(false);
int sock = -1;
sockaddr_in in_addr{};

bool reconnect() {
    #ifdef _WIN32
        closesocket(sock);
    #else
        close(sock);
    #endif

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return false;
    }
    
    in_addr.sin_family = AF_INET;
    in_addr.sin_port = htons(8080);

    #ifdef _WIN32
        in_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    #else
        inet_pton(AF_INET, "127.0.0.1", &in_addr.sin_addr);
    #endif

    while (!exit_flag) {
        if (connect(sock, (sockaddr*)&in_addr, sizeof(in_addr)) != -1) {
            std::cout << "[-] SERVER: Connected to server" << std::endl;
            return true;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));   
    }
    return false;
}

void receiver() {
    std::cout << "[-] Receiving user input" << std::endl;
    while (!exit_flag) {
        std::string input;
        if (!std::getline(std::cin, input)) {
            std::cerr << "[-] ERROR: Couldn`t complete input" << std::endl;
            exit_flag = true;
            break;
        }        
        
        if (input.length() > 64) {
            std::cerr << "[-] ERROR: Max length is 64 symbols\n";
            continue;   
        }
        bool valid = std::all_of(input.begin(), input.end(), [](char c) { return std::isdigit(c); });

        if (!valid || input.length() == 0) {
            std::cerr << "[-] ERROR: Only digits allowed\n";
            continue;
        }
        std::vector<char> cstr(input.size()+1);
        strncpy(cstr.data(), input.c_str(), input.size()+1);
        function1(cstr.data());
        std::string processed_input(cstr.data());
        {
            std::lock_guard<std::mutex> lock(m);
            buffer.push(processed_input);
            cv.notify_one();
        }
    }
}

bool is_socket_connected() {
    char buff;
    int err = recv(sock, &buff, 1, MSG_PEEK | MSG_DONTWAIT);
    if (err == 0) {
        return false;
    }
    if (err == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return true;
        }
        return false;
    }
    return true;
}

void sender() {
    while (!exit_flag) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [] { return !buffer.empty() || exit_flag; });
        if (exit_flag && buffer.empty()) break;
        while (!buffer.empty()) {
            std::string data = buffer.front();
            buffer.pop();
            lock.unlock();

            std::cout << "[-] BUFFER: " << data << std::endl;

            int sum = function2(data.c_str());
            std::string s = std::to_string(sum);

            bool sent = false;
            while(!sent && !exit_flag) {
                if (!is_socket_connected()) {
                    std::cerr << "[-] SERVER: Connection lost" << std::endl;
                    if (!reconnect()) {
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                    else {
                        std::lock_guard<std::mutex> lock(m);
                        while(!buffer.empty()) {
                            buffer.pop();
                        }
                    }
                }


                int res = send(sock, s.c_str(), s.size(), 0);

                if (res == -1) {
                    std::cerr << "[-] SERVER: Connection lost" << std::endl;
                    if (!reconnect()) {
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                }
                else {
                    sent = true;
                }
            }
        }
    }
}

int main() {
    std::locale::global(std::locale("C"));

    #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif

    while(!reconnect()) {
        std::cerr << "[-] SERVER: Failed to connect to server, retrying...";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::thread t1(receiver);
    std::thread t2(sender);

    t1.join();
    t2.join();

    #ifdef _WIN32
        closesocket(sock);
    #else
        close(sock);
    #endif

    #ifdef _WIN32
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif

    return 0;
}

