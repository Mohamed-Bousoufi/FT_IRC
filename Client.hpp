#pragma once
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <vector>
class Client
{
    public :
        int _client_id;
        struct sockaddr_storage *_client_address;
        int _client_sock;
        std :: string buff;
        std::vector<std :: string> _cmd;
        void parse_command(std :: string buff);
        Client();
        Client(int id_client,int sock,struct sockaddr_storage *);
        ~Client();
};
