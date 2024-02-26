
#include<iostream>
#include<Server.hpp>


int main(int argc, char *argv[]){

    int port = atoi(argv[1]);

    Server server(port);


    server.listenForConnections();
    server.eventLoop();

    return 0;
}