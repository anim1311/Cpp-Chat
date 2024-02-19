#include <iostream>
#include <Server.hpp> // Assuming this is correctly implemented
#include <thread>
#include <mutex>
#include <netinet/in.h> // For sockaddr_in

std::mutex mtx; // Mutex for critical section

// Function to print messages with thread safety
void printMessage(const std::string& message, const std::string& user) {
    std::lock_guard<std::mutex> lock(mtx); // Automatically releases the lock when out of scope
    std::cout << user << ": " << message << std::endl;
}

// Function to handle client communication
void handleClient(Server* server, int clientfd, sockaddr_in clientAddress, unsigned int clientLength, const std::string& user) {
    std::string message;
    while (true) {
        server->readFromSocket(clientfd);
        message = server->getBuffer();
        printMessage(message, user);
    }
}

int main() {
    Server server(8080); // Assuming Server constructor takes a port number

    int client1;
    sockaddr_in clientAddress1;
    unsigned int clientLength1 = sizeof(clientAddress1);
    std::string user1;

    int client2;
    sockaddr_in clientAddress2;
    unsigned int clientLength2 = sizeof(clientAddress2);
    std::string user2;

    // Accept first client
    server.listenForConnection();
    server.acceptConnection(&client1, &clientAddress1, &clientLength1);
    server.writeToSocket(client1, "Hello from server\n", 0);
    server.writeToSocket(client1, "Your Username: ", 0);
    server.readFromSocket(client1);
    user1 = server.getBuffer();

    // Accept second client
    server.listenForConnection();
    server.acceptConnection(&client2, &clientAddress2, &clientLength2);
    server.writeToSocket(client2, "Hello from server\n", 0);
    server.writeToSocket(client2, "Your Username: ", 0);
    server.readFromSocket(client2);
    user2 = server.getBuffer();

    std::cout << "User 1: " << user1 << std::endl;
    std::cout << "User 2: " << user2 << std::endl;

    // Create threads to handle each client
    std::thread t1(handleClient, &server, client1, clientAddress1, clientLength1, user1);
    std::thread t2(handleClient, &server, client2, clientAddress2, clientLength2, user2);

    t1.join(); // Wait for threads to finish
    t2.join();

    server.closeSocket(); // Assuming this closes all sockets properly

    return 0;
}
