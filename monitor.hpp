
#pragma once
#include "Client.hpp"
#include "Channel.hpp"
#include <map>


class monitor
{
    public :
        std::map<int, Client > _Table_client;
        std::map<int, Channel > _Table_channels;
        void new_user(int client_id,int sock,sockaddr_storage * user_addr);
        void remove_user(int fd);
        int delivery_cmd(std :: string cmd,int fd);
        void execute_cmd(int fd);
        std :: string traite_cmd(int fd);
};