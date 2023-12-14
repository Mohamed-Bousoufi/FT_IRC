#include <iostream>
#include "Channel.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <map>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include "monitor.hpp"
class Server
{
    public :
            char *_Port;
            std :: string _Password;
            int _Serv_socket;
            struct sockaddr_storage *Server_addr;
            monitor man;

            Server(char * port,char * password);
            int check_pass();
            struct addrinfo * creat_socket();
            ~Server();
            int release_server();
            void monitored_data();
            void determine_connection(std::vector<struct pollfd> &fds,int size);
};