/** 

    * @author Anirudh Madhusudhan
    * @package Server
    * @brief This class is used to create a server and to listen to incoming connections

*/


#define PRINT_IP 1


#include <Server.hpp>
#include <fstream>


Server::Server(){
    this->portNumber = 8080;
    this->clientLength = sizeof(this->serverAddress);

    std::cout << "Server created with default port number: " << this->portNumber << std::endl;

    this->createSocket();
    this->bindSocket();
    this->setupKqueue();
    
}

Server::Server(int portNumber){
    this->portNumber = portNumber;
    this->clientLength = sizeof(this->serverAddress);

    std::cout << "Server created with port number: " << this->portNumber << std::endl;

    this->createSocket();
    this->bindSocket();
    this->setupKqueue();

}

Server::~Server(){
    this->closeKqueue();
    this->closeServer();
}

int Server::createSocket(int domain, int type, int protocol){
    this->socketFD = socket(domain, type, protocol);
    if(this->socketFD < 0){
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }
    return 0;
}

int Server::bindSocket(){
    bzero((char *) &this->serverAddress, sizeof(this->serverAddress));
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;
    this->serverAddress.sin_port = htons(this->portNumber);

    if(bind(this->socketFD, (struct sockaddr *) &this->serverAddress, sizeof(this->serverAddress)) < 0){
        std::cerr << "Error binding socket" << std::endl;
        return -1;
    }
    return 0;
}

int Server::listenForConnections(int backlog){
    if(listen(this->socketFD, backlog) < 0){
        std::cerr << "Error listening for connections" << std::endl;
        return -1;
    }
    return 0;
}

int Server::acceptConnection(){
    int clientfd = accept(this->socketFD, (struct sockaddr *) &this->serverAddress, &this->clientLength);
    if(clientfd < 0){
        std::cerr << "Error accepting connection" << std::endl;
        return -1;
    }
    return clientfd;
}

int Server::readFromSocket(int clientfd){
    bzero(this->buffer, BUFFER_SIZE);
    int bytesRead = read(clientfd, this->buffer, BUFFER_SIZE);
    if(bytesRead < 0){
        std::cerr << "Error reading from socket" << std::endl;
        return -1;
    }
    return bytesRead;
}

int Server::writeToSocket(int clientfd, std::string message, int flags){
    int bytesWritten = write(clientfd, message.c_str(), message.length());
    if(bytesWritten < 0){
        std::cerr << "Error writing to socket" << std::endl;
        return -1;
    }
    return bytesWritten;
}

void Server::closeServer(){
    close(this->socketFD);
}

void Server::disconnectClient(int clientfd){
    close(clientfd);
}

void Server::setupKqueue(){
    this->kq = kqueue();
    if(this->kq < 0){
        std::cerr << "Error creating kqueue" << std::endl;
    }
}

void Server::addEvent(int fd, int filter, int flags, void *udata){
    struct kevent event;
    EV_SET(&event, fd, filter, flags, 0, 0, udata);
    if(kevent(this->kq, &event, 1, NULL, 0, NULL) < 0){
        std::cerr << "Error adding event to kqueue" << std::endl;
    }
}

void Server::removeEvent(int fd, int filter){
    struct kevent event;
    EV_SET(&event, fd, filter, EV_DELETE, 0, 0, NULL);
    if(kevent(this->kq, &event, 1, NULL, 0, NULL) < 0){
        std::cerr << "Error removing event from kqueue" << std::endl;
    }
}


void Server::eventLoop() {

    struct kevent eventSet;
    struct kevent eventList[MAX_EVENTS];
    struct timespec timeout = {10, 0}; // 10 seconds timeout

    this->addEvent(this->socketFD, EVFILT_READ, EV_ADD, NULL);

    while (true) {
        // Timeout struct could be also NULL, to have it an eternal wait.
        int nEvents = kevent(this->kq, NULL, 0, eventList, MAX_EVENTS, &timeout);

        if (nEvents < 0) {
            std::cerr << "kevent error" << std::endl;
            continue; // Might reconsider a better exit lock or signal there.
        } else if (nEvents == 0) {
            std::cout << "No activity, check or play." << std::endl;
            continue; 
        }

        for (int i = 0; i < nEvents; i++) {
            if (eventList[i].ident == static_cast<uintptr_t>(this->socketFD)) { 

                int newClientFD = this->acceptConnection();
                if (newClientFD < 0) continue; 

                this->addEvent(newClientFD, EVFILT_READ, EV_ADD, NULL);
            } else {
                int clientFD = eventList[i].ident; 
                ssize_t bytesRead = this->readFromSocket(clientFD);
                if (bytesRead < 0) {
                    std::cerr << "Issues on the data flight from FD: " << clientFD << std::endl;
                    this->disconnectClient(clientFD); 
                } else if (bytesRead == 0) {
                    this->disconnectClient(clientFD); 
                } else {
                    
                    std::string message = this->getBuffer();
                    #if PRINT_IP
                    // print the ip of the client
                    struct sockaddr_in clientAddress;
                    socklen_t clientLength = sizeof(clientAddress);
                    getpeername(clientFD, (struct sockaddr *)&clientAddress, &clientLength);
                    std::cout << "Received message from " << inet_ntoa(clientAddress.sin_addr) << ": " << message << std::endl;
                    #else
                    std::cout << "Received message: " << message << std::endl;
                    #endif
                    
                    // Read file and send the contents
                    


                    std::string header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
                    std::ifstream file("../src/test.html");
                    std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

                    std::cout << body << std::endl;
                    this->writeToSocket(clientFD, header + std::to_string(body.length()) + "\r\n\r\n" + body);
            

                }
            }
        }
    }
}



void Server::closeKqueue(){
    close(this->kq);
}

std::string Server::getBuffer() const{
    return this->buffer;
}

struct sockaddr_in Server::getServerAddress() const{
    return this->serverAddress;
}
