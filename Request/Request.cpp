#include "../includes/ServerManager.hpp"


void	ServerManager::RequestHeadersParse(Client& _client)
{
	std::string key;
	std::string value;
	for (; ;)
	{
		if (_client.buff[0] == '\r')
		{
			_client.buff.erase(0, 2);
			_client.request = true; 
			break;
		}
		key = _client.buff.substr(0, _client.buff.find(':'));
		_client.buff.erase(0, _client.buff.find(':') + 1);
		value = _client.buff.substr(1, _client.buff.find('\r') - 1);
		_client.buff.erase(0, _client.buff.find('\r') + 2);
		_client.data[key] = value;
	}
	RequestParse(_client);
}

void	ServerManager::RequestLineParse(Client& _client)
{
	_client.method = _client.buff.substr(0, _client.buff.find(' '));
	_client.buff.erase(0, _client.buff.find(' ') + 1);
	_client.path = _client.buff.substr(_client.buff.find('/') + 1, _client.buff.find(' '));
	_client.buff.erase(_client.buff.find('/'), _client.buff.find(' ') + 1);
	_client.version = _client.buff.substr(0, _client.buff.find('\n') - 1);
	if (_client.version != "HTTP/1.1")
	{
		// throw error page
		std::cout << "Wrong HTTP version!" << std::endl;
		return;
	}
	_client.buff.erase(0, _client.buff.find('\n') + 1);
	RequestHeadersParse(_client);
}

void	ServerManager::RequestParse(Client& _client)
{
	if (_client.request == false)
	{	
		_client.buff.append(_client.buffer, _client.readBytes);
		if (_client.buff.find("\r\n\r\n") != std::string::npos)
			RequestLineParse(_client);
		// RequestErrors(_client);
	}

	// else if (_client.request == true && _client.method == "POST"
	// 		&& _client.data["Transfer-Encoding"] == "chunked")
	// 	PostContentLengthChunked(_client);

	else if (_client.request == true && _client.method == "POST" 
		&& _client.flagContntLength == false)
		PostContentLength(_client);

	// CHECK IF CONTENT-LENGTH IS AVAILABLE
	// else if (_client.request == true && _client.method == "POST")
	// {
	// 	// should handle if no content length nor tranfer encoding found on the post request
	// 	std::cerr << "Error: No Content-Length nor Tranfer-Encoding" << std::endl;
	// 	return;
	// }
}
