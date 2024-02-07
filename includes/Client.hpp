#pragma once

#include "ServerManager.hpp"
#include <map>

#define BUFFER_SIZE 1024


class Client
{
    public:
        int socket;
        socklen_t   addressLenght;
        struct sockaddr_storage     clientAddress;
        char			buffer[BUFFER_SIZE + 1];
        //----------
        bool socketError;
        std::string method;
        std::string path;
        std::string version;
        std::map<std::string, std::string> data;
        Client(){};
        std::string		body;
        std::string     buff;
		bool			request;
		std::string		extension;
		size_t			readBytes;
		size_t			clientID;
		size_t			serverID;

        std::string generateRandomString(int length);
        size_t count_bodyLen;
		size_t content_length;
		bool flagContntLength;
		std::string randomString;
};