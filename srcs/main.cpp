#include "../includes/webserver.hpp"

int main(int ac, char **av)
{
    if (ac > 2){
        std::cerr << "Error: Wrong number of arguments." << std::endl;
        return 1;
    }
    try
    {
        ServerConfig file;
        TheServer crvr;
        if (av[1])
            file.get_file(av[1]);
        else
            file.get_file("configFiles/c.config");
        crvr.set_confg(file.servr);
        crvr.socket_creation();
        crvr.socketBindListen();
        crvr.AcceptConnections();
    }
    catch(std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    // (void)ac;
    // (void)av;
    // char b[4] = "H\0M";
    // std::string str;
    // str.append(b, 3);
    // write(1, str.c_str(), 3);
    // std::cout << str.length() << std::endl;
    return 0;
}
