#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

#define BUFFER_SIZE 256


class Client
{
private:
    int sockfd;
    int portNumber;
    unsigned int clientLength;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in clientAddress;
    

public:
    Client();
    Client(const char *serverIP, int portNumber);
    ~Client();

    int createSocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    int bindSocket();
    int connectToServer(const char *serverIP);
    int readFromSocket(int clientfd);
    int writeToSocket(int clientfd,std::string message, int flags = 0);
    void closeClient();



    std::string getBuffer() const;
    struct sockaddr_in getServerAddress() const;

};


    