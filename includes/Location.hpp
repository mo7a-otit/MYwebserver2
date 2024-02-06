#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <iostream>
#include <vector>

class Location{
    public:
        std::string NAME;
        std::string root;
        std::vector<std::string> allowed_methods;
        std::vector<std::string> index;
        //need to add this
        bool autoindex;
        bool upload;
        bool cgi;

        std::vector<std::string> cgi_path;
        

        std::vector<std::string> vec_dup;
        std::set<std::string> set_dup;


        Location();

        //member functions
        void fill_location(std::string, Location&);
        void initializeArray();

        /***getters***/
        void get_location_Name(std::string);
        int get_root(std::vector<std::string>);
        int get_autoindex(std::vector<std::string>);
        int get_methods(std::vector<std::string>);
        //////wssalt hnaya
        int get_index(std::vector<std::string> word);
        int get_upload(std::vector<std::string> word);
        int get_cgi(std::vector<std::string> word);
        int get_cgi_path(std::vector<std::string> word);
};




#endif