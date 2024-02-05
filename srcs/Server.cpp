#include "../includes/webserv.hpp"

void removeSpaces(std::string& line){
     line.erase(remove_if(line.begin(), line.end(), isspace),\
         line.end());
 }

Server::Server(){
    this->host = "";
    this->server_name = "";
}

/*****check brackets******/
void BracketsCheck(std::string configfile){
    std::ifstream ss;
    ss.open(configfile);
     if (!ss.is_open())
        throw std::invalid_argument("can't open file");
    
    int i = 0;
    char c;
    std::stack<char> stack;
    
    while(ss.get(c)){
        if (c == '{'){
            stack.push(c);
            i++;
        }
        else if (c == '}'){
            if (stack.empty())
                throw std::invalid_argument("Brackets");
            else
                stack.pop();
        }
    }
    if (i == 0)
        throw std::invalid_argument("There is no brackets!!");
    if (!stack.empty())
        throw std::invalid_argument("Brackets");
    ss.close();
}

//********split line************//
std::vector<std::string> split_line(std::string line, std::string delimiter){
    size_t start = 0;
    size_t end;
    std::string tmp;
    std::vector<std::string> res;
    while ((end = line.find(delimiter, start)) != std::string::npos){
        tmp = line.substr(start, end - start);
        if (!tmp.empty())
            res.push_back(tmp);
        start = end + delimiter.length();
    }
    tmp = line.substr(start);
    if(!tmp.empty())
        res.push_back(tmp);
    return res;
}

int check_is_digit(std::string word){
    for (int i = 0; word[i]; i++){
        if (!isdigit(word[i]))
            return 0;
    }
    return 1;
}

void Server::initializeArray(){

    // this->arr_derictives[0] = "listen";
    // this->arr_derictives[1] = "host";
    // this->arr_derictives[2] = "server_name";
    // this->arr_derictives[3] = "root";
    // this->arr_derictives[4] = "index";
    // this->arr_derictives[5] = "error_pages";
}

// int Server::get_root(std::vector<std::string> word){
//     if ()
// }

int Server::get_listen(std::vector<std::string> word){
    // if (word.size() < 2)
    //     throw std::invalid_argument("listen: invalid input");
    if ("listen" == word[0]){
        for (size_t i = 1; i < word.size(); i++){
            // std::cout << word[0] << " = " << word[i] << std::endl;
            if (!check_is_digit(word[i]))
                throw std::invalid_argument("listen: invalid character");
            if (std::stoi(word[i].c_str()) > 65535)
                throw std::invalid_argument("listen: value out of range");
            this->listen.push_back(std::stoi(word[i].c_str()));
        }
        return 1;
    }
    return 0;
}

int Server::get_host(std::vector<std::string> word){
    // if (word.size() != 2)
    //     throw std::invalid_argument("host: invalid input");
    if ("host" == word[0]){
        //check how much '.'
        // word[1].erase(word[1].find(';'));
        int count = 0;
        for(size_t i = 0; i < word[1].length(); i++){
            if(word[1][i] == '.')
                count++;
        }
        if (count != 3)
            throw std::invalid_argument("host: error '.'");
        std::vector<std::string> ip_add = split_line(word[1], ".");
        if (ip_add.size() != 4)
            throw std::invalid_argument("host: invalid size");
        for (size_t i = 0;i < ip_add.size(); i++){
            if(!check_is_digit(ip_add[i]))
                throw std::invalid_argument("host: invalid character");
            if (ip_add[i].length() > 3)
                throw std::invalid_argument("host: invalid range");
            if (std::stoi(ip_add[i].c_str()) > 255 || std::stoi(ip_add[i].c_str()) < 0)
                throw std::invalid_argument("host: invalid range!");
        }
        //now all good we can save our value
        this->host = word[1];
        return 1;
    }
    return 0;
}

int Server::get_server_name(std::vector<std::string> word){
        if ("server_name" == word[0]){
            // word[1].erase(word[1].find(';'));
            this->server_name = word[1];
            return 1;
        }
        return 0;
}

int Server::get_error_page(std::vector<std::string> word){
    if ("error_page" == word[0]){
        if (word.size() != 3)
            throw std::invalid_argument("error_pages: missing code or target..!");
        std::cout << word[0] << " :";
        if (word[1].length() != 3)
            throw std::invalid_argument("error_pages: invalid code");
        // if(word[2][0] != '/')
        //     throw std::invalid_argument("error_pages: invalid target");
        for (size_t i = 1; i < word.size(); i++)
            this->error_pages.push_back(word[i]);
        std::cout << word[1] << " " << word[2] << std::endl;
        return 1;
    }
    return 0;
}

int Server::get_clienMAxBodySize(std::vector<std::string> word){
    if ("client_max_body_size" == word[0]){
        // word[1].erase(word[1].find(';'));
        for (size_t i = 0; i < word[1].length(); i++)
            if (!std::isdigit(word[1][i]))
                throw std::invalid_argument("server: client_max_body_size: invalid input");
        std::stringstream ss(word[1]);
        ss >> this->max_body;
        return 1;
    }
    return 0;
}

void Server::fill_server(std::string line, Server &srvr){
    size_t posVergul = line.find(';');
    line.erase(posVergul);
    std::vector<std::string> word = split_line(line, " ");
    if (word.size() < 2)
        throw std::invalid_argument("server: invalid input");
    if (posVergul == std::string::npos && word[0] != "}")
        throw std::invalid_argument("can't find ';'");
    std::cout << "- " <<  word[0] << " = " << word[1] << std::endl;
    if (word[0] != "listen" && word[0] != "host" && word[0] != "server_name"\
        && word[0] != "error_page" && word[0] != "client_max_body_size"\
        && word[0] != "root")
        throw std::invalid_argument("server: invalid input in the server context!!!");
    if(srvr.get_listen(word)){
        srvr.duplicate_set.insert("listen");
        srvr.duplicate_vec.push_back("listen");
    }
    else if(srvr.get_host(word)){
        srvr.duplicate_set.insert("host");
        srvr.duplicate_vec.push_back("host");
    }
    else if(srvr.get_server_name(word)){
        srvr.duplicate_set.insert("server_name");
        srvr.duplicate_vec.push_back("server_name");
    }
    else if(srvr.get_error_page(word)){}
    else if(srvr.get_clienMAxBodySize(word)){
        srvr.duplicate_set.insert("client_max_body_size");
        srvr.duplicate_vec.push_back("client_max_body_size");
    }
}


void Server::get_file(std::string file){
    BracketsCheck(file);
    std::ifstream ss;
    ss.open(file);
     if (!ss.is_open())
        throw std::invalid_argument("can't open file");
    std::string line;
    while (std::getline(ss, line)){
        if(line.find("server") != std::string::npos){
            removeSpaces(line);
            if(line != "server{")
                throw std::invalid_argument("server: invalid declaration");

            Server srvr;
            std::cout << "-----------SERVER-----------" << std::endl;
            while(std::getline(ss,line) && line != "}"){
                if (line.find("location") != std::string::npos){
                    std::cout << "      --------location-------- " << std::endl;
                    Location location;
                    location.get_location_Name(line);
                    while (std::getline(ss, line) && line.find("}") == std::string::npos){
                        if (line.find(';') == std::string::npos){
                            removeSpaces(line);
                            if (!line.empty())
                                throw std::invalid_argument("invalid input outside the server context");
                        }
                        else
                            location.fill_location(line, location);
                    }
                    
                    srvr.locations.push_back(location);
                    std::cout << "      ------------------------" << std::endl;
                    if (location.root == "")
                        throw std::invalid_argument("location: root: invalid input");
                    else if(location.vec_dup.size() != location.set_dup.size())
                        throw std::invalid_argument("location: duplicate");
                }
                else if (line.find(';') == std::string::npos){
                    // std::getline(ss, line);
                    removeSpaces(line);
                    if (!line.empty())
                        throw std::invalid_argument("invalid input outside the server context");
                }
                else{
                    std::cout << "The size of locations is: " << srvr.locations.size()
                        << std::endl;
                    srvr.fill_server(line, srvr);
                }
            }
            std::cout << "----------------------------" << std::endl;
            if (srvr.listen.size() < 1 || srvr.host == "")
                throw std::invalid_argument("Serever: the server at least must have a\
                    port and host");
             if (srvr.duplicate_set.size() != srvr.duplicate_vec.size())
                throw std::invalid_argument("server: duplicate directive!");
            else
                this->servr.push_back(srvr);
                // run_server(srvr);
            // std::cout << "Waaaaa simooooo" << std::endl;
        }
        else{
            removeSpaces(line);
            if (!line.empty())
                throw std::invalid_argument("invalid input outside the server context");
        }
    }
}