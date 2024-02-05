#ifndef WEBSERVER_H
#define WEBSERVER_H

#define PORT 8080
#define BUFFER_SIZE 1024

#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <map>
#include <fstream>
#include <fcntl.h>
#include <sys/select.h>

// conf headers
#include "../includes/Server.hpp"
#include "../includes/webserv.hpp"

class	TheClient;

class   TheServer
{
	private:
		int			host_len;
		sockaddr_in	host_addr;
		std::vector<Server> confg;
		int	maxSocket;
		fd_set	readFrom_fds;
		fd_set	writeTo_fds;
		std::vector<TheClient> client_vec;
	public:
		TheServer();
		~TheServer();
		void	socket_creation();
		void	socketBindListen();
		void	AddClientsToSet();
		void	AcceptConnections();
		void	ManageClients();
		void	RequestParse(TheClient& client, int readBytes);
		void	RequestLineParse(TheClient& client);
		void	RequestHeadersParse(TheClient& client);
		void	RequestPost(TheClient& client);
		void	set_confg(const std::vector<Server>& confg);
};

class	TheClient
{
	public:
		TheClient();
		int				client_fd;
		sockaddr_in		client_addr;
		socklen_t		client_addr_len;
		char			buffer[BUFFER_SIZE + 1];
		std::string 	buff;
		std::string		method;
		std::string 	path;
		std::string 	version;
		std::map<std::string, std::string> map;
		std::string		body;
		bool			request;
		std::string		extension;
		int				readBytes;
};

#endif