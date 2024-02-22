#pragma once

/** 

    * @author Anirudh Madhusudhan
    * @package Server
    * @brief This class is used to create a server and to listen to incoming connections

*/


#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/event.h>
#include <sys/types.h>

#include <unistd.h>



#define BUFFER_SIZE 4096


class Server{
    private:

    int socketFD;
    int portNumber;
    unsigned int clientLength;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serverAddress;
    int kq; // kqueue file descriptor
    const int MAX_EVENTS = 10;

    public:

    Server();
    Server(int portNumber);
    ~Server();

    int createSocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    int bindSocket();
    int listenForConnections(int backlog = 5);

    int acceptConnection();
    int readFromSocket(int clientfd);
    int writeToSocket(int clientfd,std::string message, int flags = 0);
    void closeServer();
    void disconnectClient(int clientfd);

    void setupKqueue(); // Initialize kqueue mechanism
    void addEvent(int fd, int filter, int flags, void *udata); // Add events
    void removeEvent(int fd, int filter); // Remove specific event
    void eventLoop(); // Handle new connection and I/O with clients
    void closeKqueue(); // Close kqueue mechanism

    

    std::string getBuffer() const;
    struct sockaddr_in getServerAddress() const;


};
