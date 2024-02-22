
#include<iostream>
#include<Server.hpp>


int main(){
    Server server(8000);


    server.listenForConnections();
    server.eventLoop();

    return 0;
}