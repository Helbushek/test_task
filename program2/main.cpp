#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <clocale>

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include "./../lib/library.h"

extern "C" {
    bool function3(const char* str);
}

bool exit_flag = false;
int sock;
int client_socket;

void process_data(const std::string& s) {
    if (function3(s.c_str())) {
        std::cout << "[-] SUCCESS: Data is valid ( " << s << " )" << std::endl;
    }
    else {
        std::cerr << "[-] ERROR: Data is invalid ( " << s << " )" <<  std::endl;
    }
}

void server() {
    while(!exit_flag) {
        char buffer[1024];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        
        if (bytes_received <= 0) {
            continue;
        }
        std::string s(buffer, bytes_received);
        process_data(s);
    }   
    #ifdef _WIN32
        closesocket(sock);
    #else
        close(sock);
    #endif
}

int main() {
    std::locale::global(std::locale("C"));

    #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "[-] ERROR: Failed to create socket" << std::endl;
        return 1;
    }

    sockaddr_in in_addr{};
    in_addr.sin_family = AF_INET;
    in_addr.sin_port = htons(8080);
    
    #ifdef _WIN32
        in_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    #else
        inet_pton(AF_INET, "127.0.0.1", &in_addr.sin_addr);
    #endif

    if (in_addr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "[-] ERROR: Invalid IP address" << std::endl;
        return 1;
    }

    if (bind(sock, (sockaddr*)&in_addr, sizeof(in_addr)) == -1) {
        std::cerr << "[-] ERROR: Bind failed" << std::endl;
        return 1;
    }

    if (listen(sock, 5) == -1) {
        std::cerr << "[-] ERROR: Listening failed" << std::endl;
        return 1;
    }
    std::cout << "Server is listening..." << std::endl;

    sockaddr_in client_addr{};
    #ifdef _WIN32
        int client_addr_len;
    #else
        unsigned int client_addr_len;
    #endif

    client_addr_len = sizeof(client_addr);
    while (!exit_flag) {
        client_socket = accept(sock, (sockaddr*)&in_addr, &client_addr_len);
        if(client_socket == -1) {
            std::cerr << "[-] ERROR: Couldn`t accept connection" << std::endl;
            continue;
        }

        std::thread (server).detach();
    }

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