#include <Server.hpp>

Server::Server()
{
    this->portNumber = 8080;


    std::cout << "Server created with default port number: " << this->portNumber << std::endl;
    
    this->createSocket();

    this->bindSocket();
}

Server::Server(int portNumber)
{
    this->portNumber = portNumber;


    std::cout << "Server created with port number: " << this->portNumber << std::endl;

    this->createSocket();
    this->bindSocket();
}   

Server::~Server()
{
    std::cout << "Server destroyed" << std::endl;
    closeSocket();
}

int Server::createSocket(int domain, int type , int protocol)
{
    this->sockfd = socket(domain, type, protocol);
    if (this->sockfd < 0){
        std::cerr << "ERROR opening socket"; // Use std::cerr instead of error
        return 1;
        exit(1);
    }
    return 0;
}

int Server::bindSocket()
{
    bzero((char *) &this->serverAddress, sizeof(this->serverAddress));
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;
    this->serverAddress.sin_port = htons(this->portNumber);

    if (bind(this->sockfd, (struct sockaddr *) &this->serverAddress, sizeof(this->serverAddress)) < 0){
        std::cerr << "ERROR on binding"; // Use std::cerr instead of error
        return 1;
        exit(1);
    }
    return 0;
}

int Server::listenForConnection( int backlog_queue_size )
{
    int l = listen(this->sockfd, backlog_queue_size);
    return l;
}


int Server::acceptConnection(int *clientfd, struct sockaddr_in *clientAddress, unsigned int *clientLength)
{
    this->clientLength = sizeof(*clientAddress);
    *clientfd = accept(this->sockfd, (struct sockaddr *) clientAddress, clientLength);
    
    if (*clientfd < 0){
        std::cerr << "ERROR on accept"; // Use std::cerr instead of error
        return 1;
        exit(1);
    }

    std::cout << "Server: got connection from " << inet_ntoa(clientAddress->sin_addr) << std::endl;
    return 0;
}


int Server::readFromSocket(int clientfd)
{
    bzero(this->buffer, 256);
    int n = read(clientfd, this->buffer, 255);
    if (n < 0){
        std::cerr << "ERROR reading from socket"; // Use std::cerr instead of error
        
    }
    return n;
}

int Server::writeToSocket(int clientfd,std::string message, int flags )
{
    int n = send(clientfd, message.c_str(), message.length(),flags);
    if (n < 0){
        std::cerr << "ERROR writing to socket"; // Use std::cerr instead of error
    }
    return n;
}

void Server::closeSocket()
{
    close(this->sockfd);
}

int Server::getPortNumber() const
{
    return this->portNumber;
}

void Server::setPortNumber(int portNumber)
{
    this->portNumber = portNumber;
    closeSocket();

    this->createSocket();
    this->bindSocket();
    
}

int Server::getSocketFileDescriptor() const
{
    return this->sockfd;
}

std::string Server::getBuffer() const
{
    return this->buffer;
}

struct sockaddr_in Server::getServerAddress() const
{
    return this->serverAddress;
}

void Server::disconnectFromClient(int clientfd)
{
    close(clientfd);
}

