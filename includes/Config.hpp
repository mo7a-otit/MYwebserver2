#ifndef SERVER_HPP
#define SERVER_HPP

#include <sstream>
#include <set>

#include "Location.hpp"
class Location;

class Server{
    public:
        /*main container*/
    
        std::vector<Server> servr;
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
        Server();

        void get_file(std::string);
        void fill_server(std::string line, Server &srvr);
        
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