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

Client :: Client(int id_client,int sockt,struct sockaddr_storage *clientaddr) : _client_id(id_client),_client_address(clientaddr),_client_sock(sockt)
{
    char buff[512];
    socklen_t s = sizeof(buff);
    std :: cout << "creat client with socket"<<sockt<< std :: endl;
    inet_ntop(_client_address->ss_family,get_know_add(_client_address),buff,s);
    std :: cout << "adress client :"<<buff<< std :: endl;
}

void Client :: parse_command(std :: string buff)
{
    std :: string str = buff;
    size_t pos = str.find(' ');
    std :: string tmp;
    while(pos != std :: string ::npos)
    {
        tmp =  std :: strtok((char *)str.data()," ");
        _cmd.push_back(tmp);
        str = str.erase(0,pos + 1);
        pos = str.find(' ');
    }
    tmp =  std :: strtok((char *)str.data(),"\r\n");
    _cmd.push_back(tmp);
}

Client :: ~Client()
{
    
}