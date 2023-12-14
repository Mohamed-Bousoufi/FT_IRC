#include "Server.hpp"


int main(int argc,char **argv)
{
    if(argc == 3)
    {
        Server srv = Server(argv[1],argv[2]);
        srv.man.password = argv[2];
        srv.monitored_data();
        
        
    }
    else
        std :: cout << " ERROR : <server>  <port>"<< std:: endl;
}