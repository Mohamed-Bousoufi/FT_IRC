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
            check_regiseter(id_client);
        }
        else if(cmd[0].compare("NICK") == 0 || cmd[0].compare("nick") == 0)
        {
            if(cmd.size() >= 3)
            {
                send_msg_usr("ft_irc 4321 ",ERR_ERRONEUSNICKNAME,id_client);
                _Table_client.at(id_client)._cmd.clear();
                return;
            }
            check_nickname(cmd[1],id_client);
            check_regiseter(id_client);
            _Table_client.at(id_client)._cmd.clear();
        }
        else if(cmd[0].compare("USER") == 0 || cmd[0].compare("user") == 0)
        {
            username_check(cmd,id_client);
            check_regiseter(id_client);
            _Table_client.at(id_client)._cmd.clear();
        }
        else if(cmd[0].compare("JOIN") == 0 || cmd[0].compare("join"))
        {
            
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
         send_msg_usr("ft_irc 461 ",ERR_NEEDMOREPARAMS,id_client); 
        _Table_client.at(id_client)._cmd.clear();
    }
    else if(_Table_client.at(id_client)._had_registred == true)
    {
        send_msg_usr("ft_irc 462 ",ERR_ALREADYREGISTERED,id_client);
        _Table_client.at(id_client)._cmd.clear();
    }
    else if(password.compare(cmd))
    {
        send_msg_usr("ft_irc 464 ",ERR_PASSWDMISMATCH,id_client);
        _Table_client.at(id_client)._had_password = false;
        _Table_client.at(id_client)._cmd.clear();
    }
    else
    {
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
    if(nickname.empty())
    {
       send_msg_usr("ft_irc 431 ", ERR_NONICKNAMEGIVEN,id_client);
        _Table_client.at(id_client)._cmd.clear();
    }
    else if(nick_in_use(nickname))
    {
       send_msg_usr("ft_irc 433 ", ERR_NICKNAMEINUSE,id_client);
        _Table_client.at(id_client)._cmd.clear();
    }
    else if(valid_nickname(nickname))
    {
       send_msg_usr("ft_irc 432 ",ERR_ERRONEUSNICKNAME,id_client);
        _Table_client.at(id_client)._cmd.clear();
    }
    else
    {
        _Table_client.at(id_client)._nick_name = nickname;
        _Table_client.at(id_client)._had_nick_name = true;
    }
}

int monitor ::username_check(std :: vector<std :: string>  & cmd,int id_client)
{
    if(_Table_client.at(id_client)._had_user_name)
    {
        send_msg_usr("ft_irc 462 ",ERR_ALREADYREGISTERED,id_client);
        return(1);
    }
    if(cmd.size() < 5)
    {
        send_msg_usr("ft_irc 461",ERR_NEEDMOREPARAMS,id_client);
        return(1);
    }
    else if(cmd[1].size() > 10)
    {
        send_msg_usr("Syntax","The user name must contain no more than 10 characters.",id_client);
        return(1);
    }
    else if(cmd[2].compare("0") || cmd[3].compare("*"))
    {
        send_msg_usr(" Syntax"," : user name and real name must be separated by 0 *",id_client);
        return(1);
    }
    else if(cmd[4].find(":") != 0)
    {
        send_msg_usr ("Syntax"," : real name should be prefixed with colon(:)",id_client);
        return(1);
    }
    else
    {
        _Table_client.at(id_client)._user_name = cmd[1];
        cmd[4] = cmd[4].substr(1,cmd[4].size());
        for(size_t i = 4;i < cmd.size();i++)
        {
            _Table_client.at(id_client)._real_name.append(cmd[i]);
        }
        _Table_client.at(id_client)._had_user_name = true;

    }
    return(0);
}

void monitor::check_regiseter(int id_client)
{
    std :: string user_hosts;

    user_hosts.append(_Table_client.at(id_client)._nick_name);
    user_hosts.append("[!");
    user_hosts.append(_Table_client.at(id_client)._user_name);
    user_hosts.append("@");
    user_hosts.append(_Table_client.at(id_client)._hostname);
    user_hosts.append("]");
    if(_Table_client.at(id_client)._had_password && _Table_client.at(id_client)._had_nick_name && \
     _Table_client.at(id_client)._had_user_name && _Table_client.at(id_client)._msg_appear == false)
    {
        send_msg_usr("Welcome to the Internet Relay Network, ",user_hosts,id_client);
    }
}

void monitor::send_msg_usr(std :: string str,std :: string error,int id_client)
{
    int fd = _Table_client.at(id_client)._client_sock;
    std :: string msg = str.append(error);
    send(fd,msg.c_str(),msg.size(),0);
}
