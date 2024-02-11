#include "../includes/ServerManager.hpp"


void ServerManager::print_error(std::string msg)
{
    std::cout << msg << std::endl;
    exit(EXIT_FAILURE);
}

ServerManager::ServerManager(const Server& config)
{
    this->servers = config.servr;
}
ServerManager::~ServerManager(){}


void ServerManager::setSocketOption(int &Socket)
{
    int opt = 1;
    if (setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        print_error("Failed to set socket option");
        close(Socket);
    }
}

int ServerManager::setNonBlockingSocket(int &_socket)
{
    if (fcntl(_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
    {
        std::cerr << "Failed to set the socket as non-blocking.\n";
        return -1;
    }
    return 0;
}


void ServerManager::print_servers()
{
    for (size_t i = 0 ; i < this->servers.size(); i++)
    {
        std::cout << this->servers[i].server_name << " is listening on ports: " << this->servers[i].listen.front();
        std::cout << std::endl;
    }
}

void ServerManager::acceptIncommingConnection(int serverIndex)
{
    if (FD_ISSET(serverSockets[serverIndex], &readyToRead))
    {
        std::cout << "server index: " << serverIndex << '\n';
        Client newClient;
        newClient.addressLenght = sizeof(newClient.clientAddress);
        newClient.socket = accept(serverSockets[serverIndex], (struct sockaddr*)&newClient.clientAddress, &newClient.addressLenght);
        if (newClient.socket == -1)
            return ;
        newClient.serverID = serverIndex;
        _clients.push_back(newClient);
    }
}

void ServerManager::waitClients()
{
    FD_ZERO(&(this->readyToRead));
    FD_ZERO(&(this->readyToWrite));
    int maxSocket = serverSockets.front();
    for (size_t i = 0; i < serverSockets.size(); ++i)
    {
        FD_SET(serverSockets[i], &readyToRead);
        if (serverSockets[i] > maxSocket)
            maxSocket = serverSockets[i];
    }
    std::vector<Client>::iterator iter = this->_clients.begin();
    size_t x = 0;
    while (iter != _clients.end())
    {
        FD_SET(iter->socket, &readyToRead);
        FD_SET(iter->socket, &readyToWrite);
        if (iter->socket > maxSocket)
            maxSocket = iter->socket;
        iter->clientID = x;
        ++iter;
        x++;
    }
    if (select(maxSocket + 1, &readyToRead, &readyToWrite, 0, 0) == -1)
        print_error("Error: select failed..");
}


void ServerManager::manageClients(void)
{
    std::vector<Client>::iterator clientIter = _clients.begin();
    while (clientIter != _clients.end())
    {
        setNonBlockingSocket(clientIter->socket);
        if (FD_ISSET(clientIter->socket, &readyToRead))
        {
            if ((clientIter->readBytes = read(clientIter->socket, clientIter->buffer, BUFFER_SIZE)) < 0)
			{
				std::cerr << "Error: " << "Could not read!" << std::endl;
				clientIter = _clients.erase(clientIter);
				close (clientIter->socket);
				continue ;
			}
			clientIter->buffer[clientIter->readBytes] = '\0';
			RequestParse(*clientIter);
        }
        else  
            clientIter++;
    }
}

void ServerManager::startServer()
{
    while (true)
    {
        waitClients();
        for (size_t i = 0; i < serverSockets.size(); ++i)
            acceptIncommingConnection(i);
        manageClients();
    }
    for (size_t i = 0; i < serverSockets.size(); ++i)
        close(serverSockets[i]);
}

void ServerManager::setupServerSocket(const char *host, const char *port)
{
    struct addrinfo hints, *data;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int result = getaddrinfo(host, port, &hints, &data);
    if (result)
        print_error("Error: getaddinfo failed\n");
    int sock = socket(data->ai_family, data->ai_socktype, data->ai_protocol);
    if (sock == -1)
        print_error("Error: Socket failed\n");
    this->serverSockets.push_back(sock);
    setSocketOption(serverSockets.back());
    setNonBlockingSocket(serverSockets.back());
    if (bind(serverSockets.back(), data->ai_addr, data->ai_addrlen ))
        print_error("Error: Failed to bind.");
    if (listen(serverSockets.back(), 0))
        print_error("Error: listen failed");
    freeaddrinfo(data);
}


void ServerManager::initServers(void)
{
    std::vector<int> used_ports;
    for(unsigned int i = 0; i < this->servers.size(); ++i)
    {
        if (this->servers[i].listen.size() >= 1)
        {
            //check if the port is already used
            if (std::find(used_ports.begin(), used_ports.end(), servers[i].listen.front()) == used_ports.end())
            {
                std::stringstream tmpPort;
                tmpPort << servers[i].listen.front();
                setupServerSocket(servers[i].host.c_str(), tmpPort.str().c_str());
                used_ports.push_back(servers[i].listen.front());
                std::cout << "Server: " << i << " is listening on: " << servers[i].listen.front() << std::endl;
            }
        }
        else
            print_error("Error: server must contain at least one port");
    }
    startServer();
}