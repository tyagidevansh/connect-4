#include <iostream>
#include <winsock2.h>
#include <algorithm>
#include <cctype>
#include <cassert>
#include <sys/time.h>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
#include "position.hpp"
#include "solver.hpp"

unsigned long long getTimeMicrosec() {
    timeval NOW;
    gettimeofday(&NOW, NULL);
    return NOW.tv_sec * 1000000LL + NOW.tv_usec;    
}

//if the program cant explore all the way to the end
int getRandomScore() {
    std::srand(std::time(nullptr));
    int random = std::rand() % 7;
    return random;
}


// Process the received message and send a response
void processMessage(const std::string& message, SOCKET clientSocket) {
    std::string line = message;
    std::string response;

    if (line.length() <= 4) {
        int bestMove;
        if (getRandomScore() % 2 == 0) {
            if (line.back() < '7') { 
                bestMove = std::stoi(std::string(1, line.back())) + 1; 
            } else {
                bestMove = 4;
            }
        } else {
            if (line.back() > '1') { 
                bestMove = std::stoi(std::string(1, line.back())) - 1; 
            } else {
                bestMove = 3;
            }
    }
        response = " Time: " + std::to_string(1) + " | Best Move: " + std::to_string(bestMove) + " | Returned move: " + std::to_string(bestMove) + " | Score: " + std::to_string(0) + " | Nodes: " + std::to_string((getRandomScore()+1) * 3405);
        send(clientSocket, response.c_str(), response.length(), 0);
        return;
    }


    Solver solver;
    bool weak = false;
    Position P;

    if (P.play(line) != line.size()) {
        response = "Invalid move";  
    } else {
        unsigned long long start_time = getTimeMicrosec();
        int score = solver.solve(P, 15, weak);
        unsigned long long end_time = getTimeMicrosec();
        int bestMove = solver.getBestMove() + 1;

        if (bestMove == 1) {
            if(P.canOpponentWin(1)) {
                bestMove = 1;
                // std::srand(std::time(nullptr));
                // bestMove = (std::rand() % 6)+2;
            } else if (P.isWinningMove(1)) {
                bestMove = 1;
            } else {
                bestMove = getRandomScore() + 1;
                while (!P.canPlay(bestMove)) {
                    bestMove = getRandomScore() + 1;
                }
            }
        } 

        if (bestMove == 0){
            bestMove = getRandomScore() + 1;
            while (!P.canPlay(bestMove)) {
                bestMove = getRandomScore() + 1;
            }
        }

        if(P.canOpponentWin(1)) {
           bestMove = 1;
        }

        response = " Time: " + std::to_string(end_time - start_time) + " | Best Move: " + std::to_string(bestMove) + " | Returned move: " + std::to_string(solver.getBestMove() + 1) + " | Score: " + std::to_string(score) + " | Nodes: " + std::to_string(solver.getNodeCount());

    }
    
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
    serverAddr.sin_port = htons(3000); // same port as react

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