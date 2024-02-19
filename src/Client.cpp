#include <Client.hpp>

Client::Client()
{
    this->portNumber = 8080;
    this->clientLength = sizeof(this->clientAddress);
    std::cout << "Client Created at 127.0.0.1:8080" << this->portNumber << std::endl;
    this->createSocket();
    this->connectToServer("127.0.0.1");
}

Client::Client(const char *serverIP, int portNumber)
{
    this->portNumber = portNumber;
    this->clientLength = sizeof(this->clientAddress);
    std::cout << "Client Created at " << serverIP << ":" << this->portNumber << std::endl;
    this->createSocket();
    this->connectToServer(serverIP);
}

Client::~Client()
{
    std::cout << "Client destroyed" << std::endl;
    closeClient();
}

int Client::createSocket(int domain, int type, int protocol)
{
    this->sockfd = socket(domain, type, protocol);
    if (this->sockfd < 0)
    {
        std::cerr << "ERROR opening socket"; // Use std::cerr instead of error
        return 1;
        exit(1);
    }
    return 0;
}

int Client::connectToServer(const char *serverIP)
{
    bzero((char *)&this->clientAddress, sizeof(this->clientAddress));
    this->clientAddress.sin_family = AF_INET;
    this->clientAddress.sin_port = htons(this->portNumber);
    inet_pton(AF_INET, serverIP, &this->clientAddress.sin_addr);

    if (connect(this->sockfd, (struct sockaddr *)&this->clientAddress, sizeof(this->clientAddress)) < 0)
    {
        std::cerr << "ERROR connecting"; // Use std::cerr instead of error
        return 1;
        exit(1);
    }
    return 0;
}

int Client::readFromSocket(int clientfd)
{
    bzero(this->buffer, BUFFER_SIZE);
    int n = read(clientfd, this->buffer, BUFFER_SIZE);
    if (n < 0)
    {
        std::cerr << "ERROR reading from socket"; // Use std::cerr instead of error
        return 1;
        exit(1);
    }
    return 0;
}

int Client::writeToSocket(int clientfd, std::string message, int flags)
{
    int n = write(clientfd, message.c_str(), message.length());
    if (n < 0)
    {
        std::cerr << "ERROR writing to socket"; // Use std::cerr instead of error
        return 1;
        exit(1);
    }
    return 0;
}

void Client::closeClient()
{
    close(this->sockfd);
}

std::string Client::getBuffer() const
{
    return this->buffer;
}

struct sockaddr_in Client::getServerAddress() const
{
    return this->clientAddress;
}

