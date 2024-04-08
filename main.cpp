#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "SafeFile.hpp"

using namespace std;

const int PORT = 15000;

void handle_client(int client_socket, SafeFile& safeFile) {
    const string welcomeMsg = "Hello";
    const string waitMsg = "Wait";
    const string readyMsg = "Ready";
    const string okMsg = "Ok";

    char buffer[1024];
    ssize_t n;

    n = read(client_socket, buffer, 1023);
    if (n > 0) {
        string msg(buffer);
        if (msg.find(welcomeMsg) == 0) {
            
            send(client_socket, waitMsg.c_str(), waitMsg.length(), 0);
            this_thread::sleep_for(chrono::seconds(1));
            send(client_socket, readyMsg.c_str(), readyMsg.length(), 0);
            memset(buffer, 0, sizeof(buffer));

            n = read(client_socket, buffer, 1023);
            if (n > 0) {

                auto now = chrono::system_clock::now();
                auto now_c = chrono::system_clock::to_time_t(now);
                string timeStr = ctime(&now_c);
                timeStr.pop_back();
                string finalMsg = timeStr + ": " + string(buffer);
                safeFile.write(finalMsg);

                send(client_socket, okMsg.c_str(), okMsg.length(), 0);
            }
        }
    }
    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    SafeFile safeFile("request.txt");

    while (true) {
        cout << "Waiting for connections..." << endl;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            continue;
        }
        cout << "Connection established" << endl;
        thread clientThread(handle_client, new_socket, ref(safeFile));
        clientThread.detach();
    }

    return 0;
}
