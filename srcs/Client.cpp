#include "../includes/webserver.hpp"

TheClient::TheClient()
{
    request = false;
    client_fd = 0;
    readBytes = 0;
    memset(buffer, 0, 1025);
}
