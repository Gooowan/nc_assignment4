#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include "SafeFile.hpp"

using namespace std;

class Server {
private:
    int serverSocket;
    const int port = 15000;
    sockaddr_in serverAddress;
    SafeFile logFile;
    
    void setupServerSocket() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            cerr << "Error creating socket: " << errno << endl;
            exit(1);
        }

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
            cerr << "Bind failed with error: " << errno << endl;
            close(serverSocket);
            exit(1);
        }

        if (listen(serverSocket, SOMAXCONN) == -1) {
            cerr << "Listen failed with error: " << errno << endl;
            close(serverSocket);
            exit(1);
        }
    }

    int acceptClient() {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1) {
            cerr << "Accept failed with error: " << errno << endl;
            close(serverSocket);
            exit(1);
        }
        return clientSocket;
    }

    void handleClient(int clientSocket) {
        cout << "Connection started." << endl;
        string receivedText = receiveText(clientSocket);

        if (receivedText != "Hello") {
            sendText("First message was not 'Hello', closing connection.", clientSocket);
            cerr << "First message was not 'Hello', closing connection." << endl;
            close(clientSocket);
            return;
        }

        sendText("Wait", clientSocket);
        this_thread::sleep_for(chrono::seconds(1));
        sendText("Ready", clientSocket);
        receivedText = receiveText(clientSocket);
        
        auto now = chrono::system_clock::now();
        time_t now_c = chrono::system_clock::to_time_t(now);
        string timestamp = ctime(&now_c);
        timestamp.pop_back();  // Remove the newline character
        receivedText = timestamp + " - " + receivedText;

        logFile.write(receivedText);
        sendText("OK", clientSocket);
        close(clientSocket);
        cout << "Connection closed." << endl;
    }

    void sendText(const string& text, int clientSocket) {
        int textLength = text.size();
        send(clientSocket, &textLength, sizeof(int), 0);
        send(clientSocket, text.c_str(), text.size(), 0);
    }

    string receiveText(int clientSocket) {
        int textLength;
        recv(clientSocket, &textLength, sizeof(int), 0);
        textLength = ntohl(textLength); 
        vector<char> buffer(textLength);
        recv(clientSocket, buffer.data(), textLength, 0);
        return string(buffer.begin(), buffer.end());
    }

public:
    Server(const string& logFilePath) : logFile(logFilePath) {
        setupServerSocket();
    }

    void run() {
        vector<thread> threads;
        while (true) {
            int clientSocket = acceptClient();
            threads.emplace_back(&Server::handleClient, this, clientSocket);
        }

        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }
};

int main() {
    string logFilePath = getenv("LOG_FILE");
    Server server(logFilePath);
    server.run();
    return 0;
}
