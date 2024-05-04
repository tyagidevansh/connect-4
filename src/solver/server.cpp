#include <iostream>
#include <winsock2.h>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

    // Process the received message and send a response
    void processMessage(const std::string& message, SOCKET clientSocket) {
        // Process the message (e.g., convert to uppercase)
        std::string response = message;
        //std::transform(response.begin(), response.end(), response.begin(), ::tolower);

        // Send the response back to the client
        send(clientSocket, response.c_str(), response.length(), 0);
    }

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return -1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Bind to localhost
    serverAddr.sin_port = htons(3000); // Use any available port

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server listening on port 3000\n";

    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Inside your server loop after accepting a client connection
    while (true) {
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
        } else {
            buffer[bytesReceived] = '\0'; // Null-terminate the received data
            std::cout << "Received message from client: " << buffer << std::endl;

            // Process the message and send a response
            processMessage(buffer, clientSocket);
        }
    }


    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}

//g++ server.cpp -l ws2_32
//a.exe