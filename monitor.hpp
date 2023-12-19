
#pragma once
#include "Client.hpp"
#include "Channel.hpp"
#include <map>
#include "Errors.hpp"
#include <vector>


class monitor
{
    public :
        std :: string password;
        std::map<int, Client > _Table_client;
        std::map<int, Channel > _Table_channels;
        void new_user(int client_id,int sock,sockaddr_storage * user_addr);
        void remove_user(int fd);
        int delivery_cmd(std :: string cmd,int fd);
        void execute_cmd(int fd);
        std :: vector<std :: string >traite_cmd(int fd);
        void check_password(std :: string cmd,int id_client);
        void check_nickname(std :: string cmd,int id_client);
        int nick_in_use(std :: string nickname);
        int valid_nickname(std :: string nickname);
        int username_check(std :: vector<std :: string> & cmd,int id_client);
        void check_regiseter(int id_client);
        void send_msg_usr(std :: string msg,std :: string error,int client_id);
        void join_command(std :: vector<std :: string> cmd);
};