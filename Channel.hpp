#pragma once
#include "Client.hpp"
#include <map>

class Channel
{
    public :
        std::map<int,Client> table_users;
        std :: string _name_of_channel;
        int _id_channel;
        Channel(std :: string name_channel,int idChannel);
        ~Channel();
};