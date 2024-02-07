#include "../includes/webserv.hpp"

Location::Location(){

    // this->root = "";
}


void Location::get_location_Name(std::string line){
        if (line[line.length() -1] != '{')
            throw std::invalid_argument("locaton: can't find '{'!");
        line.erase(line.find('{'));
        std::vector<std::string> word = split_line(line, " ");
        if (word.size() != 2)
            throw std::invalid_argument("location: hada ma howa hada!!");
        if (word[1][0] != '/')
            throw std::invalid_argument("location: invalid path");
        this->NAME = word[1];
}

int Location::get_root(std::vector<std::string> word){
    if (word[0] == "root"){
        std::cout << "DONE " << std::endl;
        std::cout << "     " << word[0] << " = " << word[1] << std::endl;
        if(word.size() != 2)
            throw std::invalid_argument("location: root: invalid input");
        this->root = word[1];
        return 1;
    }
    return 0;
}

int Location::get_autoindex(std::vector<std::string> word){
    if (word[0] == "autoindex"){
        if (word.size() != 2)
            throw std::invalid_argument("location: autoindex: invalid input");
        if (word[1] != "on" && word[1] != "off")
            throw std::invalid_argument("location: autoindex: invalid input");
        else if (word[1] == "on")
            this->autoindex = true;
        else if (word[1] == "off")
            this->autoindex = false;
        std::cout << "DONE " << std::endl;
        std::cout << "     " << word[0] << " = " << word[1] << std::endl;
        return 1;
    }
    return 0;
}

int Location::get_methods(std::vector<std::string> word, Location& location){
    if (word[0] == "allowed_methods"){
        location.allowed_methods.push_back("ana");
        for (size_t i = 1; i < word.size(); i++)
            if(word[i] != "POST" && word[i] != "DELETE" && word[i] != "GET")
                throw std::invalid_argument("allowed_methods: invalid method");
        std::cout << "DONE " << std::endl;
        std::cout << "     " << word[0] << " = ";
        for (size_t i = 1; i < word.size(); i++){
            this->allowed_methods.push_back(word[i]);
            std::cout << word[i] << " ";
        }
        std::cout << std::endl;
        return 1;
    }
    return 0;
}

int Location::get_index(std::vector<std::string> word){
    if (word[0] == "index"){
        std::cout << "DONE" << std::endl;
        if(word.size() < 2)
            throw std::invalid_argument("location: index: invalid input");
        std::cout << "     " << word[0] << " = ";
        for (size_t i = 1; i < word.size(); i++){
            this->index.push_back(word[i]);
            std::cout << word[i] << " ";
        }
        std::cout << std::endl;
        return 1;
    }
    return 0;
}

int Location::get_upload(std::vector<std::string> word){
    if(word[0] == "upload"){
        if (word.size() != 2)
            throw std::invalid_argument("location: upload: invalid input");
        if (word[1] != "on" && word[1] != "off")
            throw std::invalid_argument("location: upload: invalid input");
        else if (word[1] == "on")
            this->upload = true;
        else if (word[1] == "off")
            this->upload = false;
        std::cout << "DONE" << std::endl;
        std::cout << "     " << word[0] << " = " << this->upload << std::endl;
        return 1;
    }
    return 0;
}

int Location::get_cgi(std::vector<std::string> word){
    if(word[0] == "cgi"){
        if (word.size() != 2)
            throw std::invalid_argument("location: cgi: invalid input");
        if (word[1] != "on" && word[1] != "off")
            throw std::invalid_argument("location: cgi: invalid input");
        else if (word[1] == "on")
            this->cgi = true;
        else if (word[1] == "off")
            this->cgi = false;
        std::cout << "DONE" << std::endl;
        std::cout << "     " << word[0] << " = " << this->cgi << std::endl;
        return 1;
    }
    return 0;
}

// int Location::get_cgi_path(std::vector<std::string> word){
//     // if (word[0] == "cgi_path"){
//     //     if (wor)
//     // }
// }

void Location::fill_location(std::string line, Location& location){
    if (line.find(";") == std::string::npos)
        throw std::invalid_argument("location: invalid form");
    line.erase(line.find(";"));
    std::vector<std::string> word = split_line(line, " ");
    if (word.size() < 2)
        throw std::invalid_argument("location: incorect input");
    if (location.get_root(word)){
    }
    else if(location.get_autoindex(word)){
        location.vec_dup.push_back("autoindex");
        location.set_dup.insert("autoindex");
    }
    else if(location.get_index(word)){

    }
    else if (location.get_methods(word, location)){
        location.vec_dup.push_back("allowed_methods");
        location.set_dup.insert("allowed_methods");
    }
    else if (location.get_upload(word)){}
    else if (location.get_cgi(word)){}
    // std::cout << "     " << word[0] << " = " << word[1] << std::endl;
    
}

