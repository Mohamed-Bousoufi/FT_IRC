#include "monitor.hpp"
#include <unistd.h>

void *get_addr(struct sockaddr_storage *sa)
{
    if (sa->ss_family == AF_INET) {
        return (&((struct sockaddr_in*)sa)->sin_addr);
    }
    return (&((struct sockaddr_in6*)sa)->sin6_addr);
}

void monitor::new_user(int client_id,int sock,sockaddr_storage * user_addr)
{
    char str[INET_ADDRSTRLEN];
    getnameinfo((sockaddr *)user_addr,sizeof(user_addr),str,sizeof(str),NULL,0,0);
    printf("hostname is %s\n",str);
    Client new_user(client_id,sock,user_addr);
    std :: pair<int,Client> p(client_id,new_user);
    _Table_client.insert(p);
}

int monitor::delivery_cmd(std :: string cmd,int fd)
{
    std :: map<int,Client >::iterator it;
    for(it = _Table_client.begin(); it != _Table_client.end(); it++)
    {
        if(fd == it->second._client_sock)
        {
                it->second.buff.append(cmd);
                cmd.clear();
                return(it->second._client_id);
        }
    }
    return(-1);
}

std :: string monitor::traite_cmd(int id_client)
{
    size_t pos = 0;
    std :: string  & buff = _Table_client.at(id_client).buff;
    if(buff.data() == NULL)
    {
        return(buff);
    }
    pos =  buff.find("\n");
    if(pos == std :: string ::npos)
    {
        return(buff);
    }
    if(buff.empty())
    {
        std :: cout << "it's empty cmd" << std :: endl;
        return(buff);
    }
    
    std :: string cmd;
    cmd = buff.substr(0,pos+1);
    _Table_client.at(id_client).parse_command(cmd);
    buff.erase(0,pos+1);
    return(cmd);
}

void monitor::execute_cmd(int id_client)
{

    std :: string cmd = traite_cmd(id_client);
    std :: vector<std :: string>::iterator it = _Table_client.at(id_client)._cmd.begin();
    // if(cmd == "PASS")
    // {
        
    // }
    // else
    //     std :: cout << "COMMAND NOT FOUND !" << std :: endl;
}

void monitor::remove_user(int fd)
{
    
    std :: map<int,Client> :: iterator it = _Table_client.begin();
    for(;it != _Table_client.end();it++)
    {
        if(fd == it->second._client_sock)
        {
            it->second.buff.clear();
        }
    }
}

