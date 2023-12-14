#include "Server.hpp"

Server::Server(char *port, char *password) : _Port(port), _Password(password)
{
}

void *get_in_addr(struct sockaddr_storage *sa)
{
    if (sa->ss_family == AF_INET) {
        return (&((struct sockaddr_in*)sa)->sin_addr);
    }

    return (&((struct sockaddr_in6*)sa)->sin6_addr);
}

int Server ::release_server()
{
	int ops = 1;
	struct addrinfo *sock_info;
	sock_info = creat_socket();
	if (_Serv_socket == -1)
	{
		throw "failed To creat socket";
		
	}
	setsockopt(_Serv_socket, SOL_SOCKET, SO_REUSEADDR, &ops, sizeof(int));
	if (bind(_Serv_socket, sock_info->ai_addr, sock_info->ai_addrlen) == -1)
	{
		throw "server : failed to bind";
	}
	freeaddrinfo(sock_info);
	if (listen(_Serv_socket, 10) == -1)
	{
		throw "server : failed to listen";
		
	}
	std :: cout << "server is listining" << std :: endl;
	return (_Serv_socket);
}

Server ::~Server()
{
}

struct addrinfo * Server ::creat_socket()
{
	struct addrinfo first,*result;
	std ::memset(&first, '\0', sizeof(first));
	first.ai_family = AF_UNSPEC;
	first.ai_socktype = SOCK_STREAM;
	first.ai_flags = AI_PASSIVE;

	int gtadrinfo = getaddrinfo(NULL, _Port, &first, &result);
	if (gtadrinfo != 0)
	{
		throw  gai_strerror(gtadrinfo) ;
	}
	_Serv_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (_Serv_socket == -1)
	{
		throw  "Socket Create error !";
	}
	if (fcntl(_Serv_socket, F_SETFL, O_NONBLOCK) == -1)
	{
		throw  "Can't Set Socket Non_Block";
	}
	return (result);
}

void Server ::monitored_data()
{
	try
	{
		if(check_pass())
		{
			std :: vector<struct pollfd> filed;
			int monitor;
			release_server();
			struct pollfd lisner;
			lisner.fd = _Serv_socket;
			lisner.events = POLL_IN;
			filed.push_back(lisner);
			while (1)
			{
				monitor = poll(filed.data(),filed.size(), -1);
				if (monitor < 0)
				{
					throw "server : poll error";
				}
				else
				{
					determine_connection(filed, filed.size());
				}
			}
		}
	}
	catch(const char *e)
	{
		std :: cout << e << std :: endl;
	}
}

void add_user_to_poll_table(std :: vector<pollfd> &fds,int fd)
{
		pollfd new_fds;
		new_fds.fd = fd;
		new_fds.events = POLL_IN;
		fds.push_back(new_fds);
}

void Server ::determine_connection(std :: vector<struct pollfd> &fds, int size)
{
	int client;
	int recbytes;
	
	static std :: string re;
	struct sockaddr_storage tempaddr;

	std::memset(&tempaddr, '\0', sizeof(tempaddr));
	for (int i = 0; i < size; i++)
	{		
		if (fds[i].revents == POLL_IN)
		{
			if (fds[i].fd == this->_Serv_socket)
			{
				socklen_t size_sock = sizeof(tempaddr);

				client = accept(fds[i].fd,(sockaddr *)&tempaddr, &size_sock);
				add_user_to_poll_table(fds,client);
				size = fds.size();
				man.new_user(size-1,client,&tempaddr);
			}
			else
			{
				char buff[512];
				memset(buff,'\0',sizeof(buff));
				recbytes = recv(fds[i].fd,buff, sizeof(buff),0);
				if (recbytes <= 0)
				{
					close(fds[i].fd);
					man.remove_user(fds[i].fd);
					fds[i] = fds[size - 1];
					fds.pop_back();
					std ::cout << "connection ended" << std :: endl;
				}
				else
				{
					int id_client;
					id_client = man.delivery_cmd(buff,fds[i].fd);
					if(id_client != -1)
						man.execute_cmd(id_client);
					
				}
			}
		}
	}
}

int Server :: check_pass()
{
	if(_Password.empty() || _Password.find(" ") != std :: string :: npos)
	{
		throw "Password Is Invalid";
		return(0);
	}
	else
		return(1);
}