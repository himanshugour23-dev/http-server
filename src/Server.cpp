#include "Server.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

Server::Server(int port):port(port), serverFd_(-1) {} 
Server::~Server(){
    if(serverFd_ != -1){
        close(serverFd_);
    }
}

void Server::createSocket(){
    serverFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFd_ == -1){
        throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
    }
    //Quick Restart of the server after a crash add it later 

}

void Server::bindSocket(){
    sockaddr_in serverAddress{} ; 
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if(bind(serverFd_,reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1){
        throw std::runtime_error("Failed to bind socket: " + std::string(strerror(errno)));
    }
    std::cout << "Server is bound to port " << port << std::endl;
}

void Server::startListening(){
    if(listen(serverFd_, SOMAXCONN) == -1){
        throw std::runtime_error("Failed to listen on socket: " + std::string(strerror(errno)));
    }
    std::cout << "Server is listening for incoming connections..." << std::endl;
}

void Server::handleClient(int clientFd) {
    char buffer[4096];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = recv(clientFd,buffer,sizeof(buffer) - 1,0);
    if (bytesRead == -1) {
        std::cerr<< "[Client] recv() failed: "<< std::strerror(errno)<< '\n';
        close(clientFd);
        return;
    }
    if (bytesRead == 0) {
        std::cout<< "[Client] Client disconnected\n";
        close(clientFd);
        return;
    }
    std::cout<< "\n========== REQUEST ==========\n";
    std::cout.write(buffer, bytesRead);
    std::cout<< "\n=============================\n";
    const char* body =
        "Hello World!";
    const std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello World!";
    ssize_t bytesSent = send(clientFd,response.c_str(),response.size(),0);
    if (bytesSent == -1) {
        std::cerr<< "[Client] send() failed: "<< std::strerror(errno)<< '\n';
    }
    close(clientFd);
}

void Server::start(){
    createSocket();
    bindSocket();
    startListening();
    // while*(true){

    // }
}