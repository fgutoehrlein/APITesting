#include "HttpResponder.h"

/**
 * member function which gets the current time in the correct http format.
 * @return a std::string which contains http date
 */
std::string HttpResponder::HttpResponder::GetTime() {
	using namespace std::chrono;
	auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());

	//Convert date to string
	char timeCString[50];
	// https://www.cplusplus.com/reference/ctime/strftime/
	auto ptm = gmtime(&timenow);
	strftime(timeCString, 50, "%a, %d %b %G %H:%S:%M", ptm);
	if (timeCString == nullptr)
	{
		return "";
	}

	size_t len = strlen(timeCString);
	if (len > INT_MAX)
	{
		throw std::overflow_error("data is larger than INT_MAX");
	}

	int timeStringLen = static_cast<int>(len);
	std::string timeString(timeCString, timeStringLen);
	timeString.append(" GMT");
	return timeString;
}

/**
 * member function which creates the header of the response.
 * @param contentType a std::string argument - Http Content-Type.
 * @param httpBody a std::string argument - Http body content.
 * @return a std::string which contains the http header
 */
std::string HttpResponder::CreateHeader(std::string contentType, std::string httpBody)
{
	std::string header;

	//append with http version and status code
	header.append("HTTP/" + this->HttpVersion + " " + StatusOk + "\r\n");	

	//append with date
	//header.append("Date: " + GetTime() + "\r\n");
	header.append("Server: C++ Socket\r\n");

	//get response body lenth
	//int httpBodySize = static_cast<int>(httpBody.size());

	//header.append("Content - Length: "+ std::to_string(httpBodySize) + " \r\n");

	header.append("Content-Type: ");
	if (contentType.empty()) {
		header.append("text/plain; charset=UTF-8\r\n");
	}
	else {
		header.append(contentType + "\r\n");
	}
	header.append("Connection: close\r\n\r\n");

	return header;
}

std::string HttpResponder::CreateBody()
{
	std::string body = "\r\n";
	body.append("<html>\n"
		"<body>\n"
		"<h1>Hello, World!</h1>\n"
		"</body>\n"
		"</html>\r\n");
	return body;
}

std::string HttpResponder::CreateResponse()
{
	auto body = CreateBody();
	auto header = CreateHeader("text/html; charset=UTF-8", body);

	string response;

	response.append(header);

	if (body.empty()) {
		return response;
	}
	response.append(body);
	return response;
}

HttpResponder::HttpResponder() {

}

HttpResponder::~HttpResponder() {
}
