#include "Client.hpp"


void *get_know_add(struct sockaddr_storage *sa)
{
    if (sa->ss_family == AF_INET) {
        return (&((struct sockaddr_in*)sa)->sin_addr);
    }

    return (&((struct sockaddr_in6*)sa)->sin6_addr);
}

Client :: Client(int id_client,int sockt,struct sockaddr_storage *clientaddr) : _client_id(id_client) ,_client_address(clientaddr),_client_sock(sockt)
{
    this->_had_password = false;
    this->_had_user_name = false;
    this->_had_nick_name = false;
    this->_had_registred = false;
    this->_msg_appear = false;
}

void Client :: parse_command(std :: string buff)
{
    std :: string  & str = buff;
    char *tmp = std :: strtok((char *)str.data()," \r\n");
    while(tmp != NULL)
    {
        if(tmp)
            _cmd.push_back(tmp);
        tmp =  std :: strtok(NULL," \r\n");
    }
}

Client :: ~Client()
{
    
}