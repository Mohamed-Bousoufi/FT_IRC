#pragma once
#include "Client.hpp"
#include <map>

class Channel
{
    public :
        std::map<int,Client> table_users;
        Channel();
        ~Channel();
};