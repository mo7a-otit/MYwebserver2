#include "../includes/webserver.hpp"

TheClient::TheClient()
{
    request = false;
    client_fd = 0;
    readBytes = 0;
    memset(buffer, 0, 1025);
    flagContntLength = false;
    count_bodyLen = 0;
    randomString = generateRandomString(5);
}
