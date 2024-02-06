#ifndef WEBSERV_HPP
#define WEBSERV_HPP



#include <sys/time.h>
#include <vector>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <exception>
#include <map>
#include <stack>


// #define PORT 1212
#define BUFFER_SIZE 1024

#include "ServerConfig.hpp"
#include "Location.hpp"


std::vector<std::string> split_line(std::string line,\
                             std::string delimiter);



#endif