#include "../includes/ServerManager.hpp"



int main(int ac, char **av)
{
    (void)ac;

        try

        
        {
            Server file;
            file.get_file(av[1]);
            ServerManager server(file);
            server.initServers();
        }
        catch(std::exception& e)
        {
            std::cout << "Error: " << e.what()
                << std::endl;
            return 1;
        }
    return 0;
}