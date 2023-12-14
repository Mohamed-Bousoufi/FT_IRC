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

    Client new_user(client_id,sock,user_addr);
    new_user._hostname = str;
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

std :: vector<std :: string> monitor::traite_cmd(int id_client)
{
    size_t pos = 0;
    std :: string  & buff = _Table_client.at(id_client).buff;
    if(buff.data() == NULL)
    {
        return(_Table_client.at(id_client)._cmd);
    }
    pos =  buff.find("\n");
    if(pos == std :: string ::npos)
    {
        return(_Table_client.at(id_client)._cmd);
    }
    if(buff.empty())
    {
        std :: cout << "it's empty cmd" << std :: endl;
        return(_Table_client.at(id_client)._cmd);
    }
    
    std :: string cmd;
    cmd = buff.substr(0,pos+1);
    _Table_client.at(id_client).parse_command(cmd);
    buff.erase(0,pos+1);
    return(_Table_client.at(id_client)._cmd);
}

void monitor::execute_cmd(int id_client)
{

    std::vector <std :: string> cmd = traite_cmd(id_client);
    if(cmd.size() > 0)
    {
        
        if(cmd[0].compare("PASS") == 0 || cmd[0].compare("pass") == 0)
        {
            check_password(cmd[1],id_client);
        }
        else if(cmd[0].compare("NICK") == 0 || cmd[0].compare("nick") == 0)
        {
            if(cmd.size() >= 3)
            {
                std :: cout << "ft_irc 432 "<<ERR_ERRONEUSNICKNAME<< std :: endl;
                _Table_client.at(id_client)._cmd.clear();
                return;
            }
            check_nickname(cmd[1],id_client);
        }
        else
        {
            std :: vector<std :: string>::iterator it = cmd.begin();
            for(; it != cmd.end();it++)
            {
                std :: cout << *it << std :: endl;
            }
            std :: cout << "-------------" << std :: endl;
            _Table_client.at(id_client)._cmd.clear();

        }
    }
}

void monitor::remove_user(int fd)
{
    
    std :: map<int,Client> :: iterator it = _Table_client.begin();
    for(;it != _Table_client.end();it++)
    {
        if(fd == it->second._client_sock)
        {
            it->second.buff.clear();
            it->second._cmd.clear();
        }
    }
}

void monitor :: check_password(std :: string cmd,int id_client)
{
    if(cmd.empty())
    {
        std :: cout << "ft_irc 461 "<< ERR_NEEDMOREPARAMS << std :: endl;
        _Table_client.at(id_client)._cmd.clear();
    }
    else if(_Table_client.at(id_client)._had_registred == true)
    {
        std :: cout << "ft_irc 462 " << ERR_ALREADYREGISTERED << std :: endl;
        _Table_client.at(id_client)._cmd.clear();
    }
    else if(password.compare(cmd))
    {
        std :: cout << "ft_irc 464 "<< ERR_PASSWDMISMATCH << std :: endl;
        _Table_client.at(id_client)._had_password = false;
        _Table_client.at(id_client)._cmd.clear();
    }
    else
    {
        std :: cout << "PASSWORD CORRECT" << std :: endl;
        _Table_client.at(id_client)._had_password = true;
        _Table_client.at(id_client)._cmd.clear();
    }
        
}

int monitor::nick_in_use(std :: string nickname)
{
    std :: string tmp;
    std :: map<int,Client> :: iterator it = _Table_client.begin();
    for(;it != _Table_client.end();it++)
    {
        tmp = it->second._nick_name;
        if(tmp.compare(nickname) == 0)
        {
            return(1);
        }
        tmp.clear();
    }
    return(0);
}

int monitor::valid_nickname(std :: string nickname)
{
    std :: string str = "0123456789:#";
    int pos;
    for(size_t i =0;i<str.size();i++)
    {
        pos = nickname.find(str[i]);
        if(pos == 0)
        {
            return(1);
        }
    }
    return(0);
}

void monitor::check_nickname(std :: string nickname,int id_client)
{
    std :: cout << nickname << std :: endl;
    if(nickname.empty())
    {
        std :: cout << "ft_irc 431 "<< ERR_NONICKNAMEGIVEN << std :: endl;
        _Table_client.at(id_client)._cmd.clear();
    }
    else if(nick_in_use(nickname))
    {
        std :: cout << "ft_irc 433 "<< ERR_NICKNAMEINUSE<< std :: endl;
        _Table_client.at(id_client)._cmd.clear();
    }
    else if(valid_nickname(nickname))
    {
        std :: cout << "ft_irc 432 "<<ERR_ERRONEUSNICKNAME<< std :: endl;
        _Table_client.at(id_client)._cmd.clear();
    }
    else
    {
        _Table_client.at(id_client)._nick_name = nickname;
        _Table_client.at(id_client)._had_nick_name = true;
        std :: cout << nickname << "@"<<_Table_client.at(id_client)._hostname<< std :: endl;
        _Table_client.at(id_client)._cmd.clear();
    }
}

