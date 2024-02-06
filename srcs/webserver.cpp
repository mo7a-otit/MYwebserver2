#include "../includes/webserver.hpp"

TheServer::TheServer()
{
    host_len = 0;
	maxSocket = 0;
}

TheServer::~TheServer()
{
	for (size_t x = 0; x < confg.size(); x++)
    	close (confg[x].serverSocket);
}

void	TheServer::set_confg(const std::vector<ServerConfig>& confg)
{
	this->confg = confg;
}

void    TheServer::socket_creation()
{
	for (size_t x = 0; x < confg.size(); x++)
	{
		confg[x].serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (confg[x].serverSocket < 0)
		{
			std::cerr << "Error: " << strerror(errno) << std::endl;
			exit (EXIT_FAILURE);
		}
    	if (fcntl(confg[x].serverSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
		{
			std::cerr << "Error: " << strerror(errno) << std::endl;
			exit (EXIT_FAILURE);
		}
	}
	// std::cout << "Server Sockets created successfully!" << std::endl;   
}

void    TheServer::socketBindListen()
{
	for (size_t x = 0; x < confg.size(); x++)
	{
		host_len = sizeof(host_addr);

		host_addr.sin_family = AF_INET;
		host_addr.sin_port = htons(confg[x].listen.front()); // Convert 16-bit integers from the host byte order to network byte order.
		host_addr.sin_addr.s_addr = inet_addr(confg[x].host.c_str()); // Convert the 32-bit integers from the host byte order to network byte order.
		int	p = 1;
		setsockopt(confg[x].serverSocket, SOL_SOCKET, SO_REUSEADDR, &p, sizeof(int));
		if (bind(confg[x].serverSocket, (sockaddr *)&host_addr, host_len) < 0)
		{
			std::cerr << "Error: " << strerror(errno) << std::endl;
			exit (EXIT_FAILURE);
		}
		fcntl(confg[x].serverSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
		// std::cout << "SOCKET successfully bound to address!" << std::endl;
		if (listen(confg[x].serverSocket, SOMAXCONN) < 0)
		{
			std::cerr << "Error: " << strerror(errno) << std::endl;
			exit (EXIT_FAILURE);
		}
		std::cout << "Server " << x << " listening on PORT: " << confg[x].listen.front() << std::endl;
	}
}

void    TheServer::AcceptConnections()
{
	while (true)
	{
		maxSocket = confg[0].serverSocket;
		FD_ZERO(&readFrom_fds);
		FD_ZERO(&writeTo_fds);
	
		//adding server sockets to the set
		for (size_t x = 0; x < confg.size(); x++)
		{
			FD_SET(confg[x].serverSocket, &readFrom_fds);
			if (confg[x].serverSocket > maxSocket)
				maxSocket = confg[x].serverSocket;
		}
		// adding clients to the set
		AddClientsToSet();
		if (select(maxSocket + 1, &readFrom_fds, &writeTo_fds, NULL, 0) < 0)
		{
			std::cerr << "Error: " << strerror(errno) << std::endl;
			continue;
		}
		for (size_t x = 0; x < confg.size(); x++)
		{
			if (FD_ISSET(confg[x].serverSocket, &readFrom_fds))
			{
				TheClient newClient;
				newClient.client_addr_len = sizeof(newClient.client_addr);
				newClient.client_fd = accept(confg[x].serverSocket, (sockaddr*)&newClient.client_addr, &newClient.client_addr_len);
				if (newClient.client_fd < 0)
				{
					std::cerr << "Error: " << strerror(errno) << std::endl;
					continue;
				}
				client_vec.push_back(newClient);
			}
		}
		ManageClients();
	}
}

void	TheServer::ManageClients()
{
	std::string response = "HTTP/1.1 200 OK\r\n"
							"Server: CRVR\r\n"
							"Content-type: text/html\r\n\r\n"    // just for testing purposes
							"<html>hello, world</html>\r\n";
	std::vector<TheClient>::iterator it = client_vec.begin();
	for (; it != client_vec.end(); it++)
	{
		if (fcntl(it->client_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
		{
			std::cerr << "Error: " << strerror(errno) << std::endl;
			exit (EXIT_FAILURE);
		}
		if (FD_ISSET(it->client_fd, &readFrom_fds))
		{
			// read from the socket
			if ((it->readBytes = read(it->client_fd, it->buffer, BUFFER_SIZE)) < 0)
			{
				std::cerr << "Error: " << "Could not read!" << std::endl;
				it = client_vec.erase(it);
				close (it->client_fd);
				return;
			}
			it->buffer[it->readBytes] = '\0';
			// std::cout << confg[0].locations[0].root << std::endl;
			// std::cout << "Buff = " << it->buffer << std::endl;
			// it->buff.append(buffer, it->readBytes);
			RequestParse(*it, it->readBytes);
		}
		// if (FD_ISSET(it->client_fd, &writeTo_fds))
		// {
		// 	// write to the socket
		// 	if (write (it->client_fd, response.c_str(), response.length()) < 0)
		// 	{
		// 		std::cerr << "Error: " << "Could not write!" << std::endl;
		// 		it = client_vec.erase(it);
		// 		close (it->client_fd);
		// 		return ;
		// 	}
		// 	// Handle response
		// 	it = client_vec.erase(it);
		// 	close (it->client_fd);
		// 	return;
		// }
	}
}

void	TheServer::AddClientsToSet()
{
	std::vector<TheClient>::iterator it = client_vec.begin();
	for (; it != client_vec.end(); it++)
	{
		FD_SET(it->client_fd, &readFrom_fds);
		FD_SET(it->client_fd, &writeTo_fds);
		if (it->client_fd > maxSocket)
			maxSocket = it->client_fd;
	}
}

void	TheServer::RequestPost(TheClient& client)
{
	// if(client.map["Content-Length"])
	std::stringstream str(client.map["Content-Length"]);
	str >> client.content_length;
	std::string type = client.map["Content-Type"];
	std::string ext = type.substr(type.find('/')+1, type.length());
	if (ext == "plain")
		ext = "txt";
	std::ofstream file("uploads/file."+ext, std::ios::app);
	if(!file.is_open()){
		std::cerr << "Can't open the file" << std::endl;
		return;
	}
	else{
		if(!client.buff.empty()){
			file.write(client.buff.c_str(), client.buff.size());
			client.count_bodyLen = client.buff.size();
			client.buff.erase();
		}
		else{
			file.write(client.buffer, client.readBytes);
			client.count_bodyLen += client.readBytes;
		}
		std::cout << client.count_bodyLen << " = " 
			<< client.content_length << std::endl;
		if(client.count_bodyLen == client.content_length){
			std::cout << client.content_length << " = " 
				<< client.count_bodyLen << std::endl;
			std::cout << "file closed" << std::endl;
			file.close();
			client.flagContntLength = true;
		}
	}

}
