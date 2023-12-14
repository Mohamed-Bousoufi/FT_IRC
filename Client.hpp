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
        std :: string _client_name;
        std :: string _hostname;
        std :: string _nick_name;
        int _client_id;
        struct sockaddr_storage *_client_address;
        int _client_sock;
        std :: string buff;
        bool _had_password;
        bool _had_nick_name;
        bool _had_user_name;
        bool _had_registred;
        std::vector<std :: string> _cmd;
        void parse_command(std :: string buff);
        Client(int id_client,int sock,struct sockaddr_storage *);
        ~Client();
};
