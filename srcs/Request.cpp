#include "../includes/webserver.hpp"

void	TheServer::RequestParse(TheClient& client, int readBytes)
{
	if (client.request == false)
	{	
		client.buff.append(client.buffer, readBytes);
		if (client.buff.find("\r\n\r\n") != std::string::npos)
			RequestLineParse(client);
	}
	else if (client.request == true && client.method == "POST" 
		&& client.flagContntLength == false)
	{
		RequestPost(client);
	}
}

void	TheServer::RequestLineParse(TheClient& client)
{
	client.method = client.buff.substr(0, client.buff.find(' '));
	client.buff.erase(0, client.buff.find(' ') + 1);
	// std::cout << "client.method: " << client.method << std::endl;
	client.path = client.buff.substr(client.buff.find('/') + 1, client.buff.find(' '));
	client.buff.erase(client.buff.find('/'), client.buff.find(' ') + 1);
	// std::cout << "client.path: " << client.path << std::endl;
	client.version = client.buff.substr(0, client.buff.find('\n') - 1);
	if (client.version != "HTTP/1.1")
	{
		// throw error page
		std::cout << "Wrong HTTP version!" << std::endl;
		return;
	}
	client.buff.erase(0, client.buff.find('\n') + 1);
	// std::cout << "client.version = " << client.version << std::endl;
	RequestHeadersParse(client);
}

void	TheServer::RequestHeadersParse(TheClient& client)
{
	std::string key;
	std::string value;
	for (; ;)
	{
		if (client.buff[0] == '\r')
		{
			client.buff.erase(0, 2);
			client.request = true;
			break;
		}
		key = client.buff.substr(0, client.buff.find(':'));
		client.buff.erase(0, client.buff.find(':') + 1);
		value = client.buff.substr(1, client.buff.find('\r') - 1);
		client.buff.erase(0, client.buff.find('\r') + 2);
		client.map[key] = value;
	}
	if (client.request == true && client.method == "POST"
			&& client.flagContntLength == false)
		RequestPost(client);
}
