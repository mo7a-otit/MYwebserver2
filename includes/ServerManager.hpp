#pragma once


#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <fcntl.h>
#include <netdb.h>
#include "Client.hpp"
#include <map>


//othman header's
#include "../includes/Config.hpp"

#define MAX_REQUEST_SIZE 4096
#define MAX_CHUNKED_SIZE 500

class Client;


class ServerManager
{
    private:
        std::vector<Server> servers;
        std::vector<int> serverSockets;
        fd_set readyToRead, readyToWrite;
        std::vector<Client> _clients;
    public:
        ServerManager(const Server& config);
        ~ServerManager();
        void acceptIncommingConnection(int index);
        void manageClients(void);
        void setupServerSocket(const char *host, const char *port);
        void setSocketOption(int &Socket);
        void initServers();
        int setNonBlockingSocket(int &_socket);
        void print_error(std::string msg);
        void print_servers();
        void startServer();
        void waitClients();
        void readClientHeader(Client &_client);
        std::string extractMethod(std::string &request);
        // request parsing funcs
        void    RequestParse(Client& _client);
        void    RequestHeadersParse(Client& _client);
        void    RequestLineParse(Client& _client);
        void	PostContentLength(Client& _client);
};