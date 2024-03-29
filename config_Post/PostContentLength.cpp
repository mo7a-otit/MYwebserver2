

#include "../includes/webserv.hpp"

std::string Client::generateRandomString(int length) {
    std::srand(std::time(0));
    const char characters[] = "abcdefghijklmnopqrstuvwxyz";
    int numCharacters = sizeof(characters) - 1;
    std::string randomString;

    for (int i = 0; i < length; ++i) {
        int randomIndex = std::rand() % numCharacters;
        randomString += characters[randomIndex];
    }
    return randomString;
}
std::string FileType(std::string path){
    if (path == "css")
        return (".css");
    else if (path == "csv")
        return (".csv");
    else if (path == "gif")
        return (".gif");
    else if (path == "html")
        return (".htm");
    else if (path == "html" || path == ".php")
        return (".html");
    else if (path == "x-icon")
        return (".ico");
    else if (path == "jpeg")
        return (".jpeg");
    else if (path == "jpeg")
        return (".jpg");
    else if (path == "javascript")
        return (".js");
    else if (path == "json")
        return (".json");
    else if (path == "png")
        return (".png");
    else if (path == "pdf")
        return (".pdf");
    else if (path == "svg+xml")
        return (".svg");
    else if (path == "plain")
        return (".txt");
    else if (path == "mp4")
        return (".mp4");
    else if (path == "webm")
        return (".WebM");
    else if (path == "ogg")
        return (".Ogg");
    else if (path == "x-msvideo")
        return (".AVI");
    else if (path == "mpeg")
        return (".MPEG");
    else if (path == "zip")
        return (".zip");
    else if (path == "tiff")
        return (".tiff");
    return NULL;
}

void	ServerManager::PostContentLength(Client& client)
{
	// if(client.data["Content-Length"])
	std::stringstream str(client.data["Content-Length"]);
	str >> client.content_length;
	std::string type = client.data["Content-Type"];
	std::string filename = FileType(type.substr(type.find('/')+1, type.length()));

	std::ofstream file("uploads/" + client.randomString + filename, std::ios::app);
	if(!file.is_open()){
		std::cerr << "Can't open the file" << std::endl;
		return;
	}
	else{
		if(!client.buff.empty()){
			file.write(client.buff.c_str(), client.buff.size());
			client.count_bodyLen = client.buff.size();
			client.buff.erase();
		}
		else{
			file.write(client.buffer, client.readBytes);
			client.count_bodyLen += client.readBytes;
		}
		std::cout << client.count_bodyLen << " = " 
			<< client.content_length << std::endl;
		if(client.count_bodyLen == client.content_length){
			// std::cout << client.content_length << " = " 
			// 	<< client.count_bodyLen << std::endl;
			std::cout << "file closed" << std::endl;
			file.close();
			client.flagContntLength = true;
		}
	}
}