#include "Client.hpp"


void *get_know_add(struct sockaddr_storage *sa)
{
    if (sa->ss_family == AF_INET) {
        return (&((struct sockaddr_in*)sa)->sin_addr);
    }

    return (&((struct sockaddr_in6*)sa)->sin6_addr);
}


Client :: Client()
{

}

Client :: Client(int id_client,int sockt,struct sockaddr_storage *clientaddr) 
{

    this->_client_id = id_client;
    this->_client_sock = sockt;
    this->_client_address = clientaddr;
    // char buff[512];
    // socklen_t s = sizeof(buff);
    // std :: cout << "creat client with socket"<<sockt<< std :: endl;
    // inet_ntop(_client_address->ss_family,get_know_add(_client_address),buff,s);
    // std :: cout << "adress client :"<<buff<< std :: endl;
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