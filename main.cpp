#include <iostream>
#include <Server.hpp>


int main() {
    
    Server server(8080);

    int clientfd;
    struct sockaddr_in clientAddress;
    unsigned int clientLength;


    server.listenForConnection();
    server.acceptConnection(&clientfd, &clientAddress, &clientLength);
    server.writeToSocket(clientfd,"What is your name?");
    server.readFromSocket(clientfd);
    server.writeToSocket(clientfd,"Hello, " + server.getBuffer());
    server.disconnectFromClient(clientfd);

    
    return 0;
}

