#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <sstream>
#include <set>

#include "Location.hpp"
class Location;

class ServerConfig{
    public:
        /*main container*/
    
        std::vector<ServerConfig> servr;
    /*this contairer contain all of this*/
    //  |||||||||||||||||
    //  VVVVVVVVVVVVVVVVV
        int serverSocket;
        std::vector<int> listen;
        std::string root;
        std::string host;
        std::string server_name;
        long long max_body;
        std::vector<std::string> error_pages;
        std::vector<Location> locations;
        std::set<std::string> duplicate_set;
        std::vector<std::string> duplicate_vec;

    public:
        ServerConfig();

        void get_file(std::string);
        void fill_server(std::string line, ServerConfig &srvr);
        
        /*****members*****/
        void initializeArray();

        /****getters****/
        // int get_root(std::vector<std::string> word);
        int get_listen(std::vector<std::string> word);
        int get_host(std::vector<std::string> word);
        int get_server_name(std::vector<std::string> word);
        int get_error_page(std::vector<std::string> word);
        int get_clienMAxBodySize(std::vector<std::string> word);
};



#endif