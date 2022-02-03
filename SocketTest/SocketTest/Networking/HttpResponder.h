#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "HttpStatusCode.h"

class HttpResponder {
public:
	HttpResponder();

	~HttpResponder();

	std::string GetTime();
//Get ServerName
	std::string GetServerName();
	std::string GetServerName(std::string serverName);

//Content-Type
	std::string GetContentType();
	std::string GetContentType(std::string contentType);

//Connection Type (Closed, keep-alive)
	std::string GetConnectionType();
	std::string GetConnectionType(std::string connectionType);

//Get Content Lenth
	int GetContentLenth(std::string content);
	int GetContentLenth(char content[]);

	// Create Header
	std::string CreateHeader(std::string contentType, std::string httpBody);
	// Create Body
	std::string CreateBody();
	// Create Response
	std::string CreateResponse();

private:
	const std::string HttpVersion = "1.1";
};