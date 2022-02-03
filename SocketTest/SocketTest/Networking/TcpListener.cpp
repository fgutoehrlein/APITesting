#include "TcpListener.h"

TcpListener::TcpListener(std::string ipAddress, int port, MessageRecievedHandler handler, bool verbose)
	:m_ipAddress(ipAddress), m_port(port), MessageRecieved(handler), Verbose(verbose) {

}

TcpListener::~TcpListener() {
	CleanUp();
}

/**
 * a normal member using the client SOCKET and a message to send it.
 * @param clientSocket a SOCKET argument.
 * @param message a std::string argument.
 * @return the listening SOCKET or -1 if failed
 */
void TcpListener::Send(int clientSocket, std::string message) {

	HttpResponder responseCreater;

	auto response = responseCreater.CreateResponse();
	auto response_cstr = response.c_str();

	send(clientSocket, response.c_str(), response.size() + 1, 0);

	if (this->Verbose) {
		std::cout << "Recieved Bytes: " << message.size() + 1 << "| Message: " << message << std::endl;
	}

	if (this->Verbose) {
		std::cout << " Sending Bytes: " << response.size() + 1 << "| Message: " << response << std::endl;
	}
	closesocket(clientSocket);
}

/**
 * a normal member initializing winsock.
 */
bool TcpListener::Init() {
	WSADATA wsData;
	WORD winsockVersion = MAKEWORD(2, 2);

	int winsockOk = WSAStartup(winsockVersion, &wsData);
	if (winsockOk != 0) {
		std::cerr << "Cant init Winsock!";
		return false;
	}
	return true;
}

//The main processing loop

void TcpListener::Run() {

	char buffer[MAX_SOCKET_BUFFER_SIZE];

	while (true) {
		//Create a listening socket
		SOCKET listeningSocket = CreateSocket();
		if (listeningSocket == INVALID_SOCKET) {
			break;
		}


		sockaddr_in clientAddr;
		SOCKET clientSocket = WaitForConnection(listeningSocket, &clientAddr);

		if (clientSocket == -1) {
			break;
		}

		HandleClientSocket(clientSocket, clientAddr, listeningSocket, buffer);
	}
}

void TcpListener::HandleClientSocket(const SOCKET& clientSocket, const sockaddr_in& clientAddr, const SOCKET& listeningSocket, char  buffer[49152])
{
	if (this->Verbose) {
		VerboseConnection(clientAddr);
	}

	closesocket(listeningSocket);

	int bytesRecieved = 0;

	do {
		ZeroMemory(buffer, MAX_SOCKET_BUFFER_SIZE);
		bytesRecieved = recv(clientSocket, buffer, MAX_SOCKET_BUFFER_SIZE, 0);

		if (bytesRecieved > 0) {
			if (this->MessageRecieved == NULL) {
				std::cerr << "Cant send message! MessageRecieved Callback is NULL";
				return;
			}
			if (this->MessageRecieved != NULL) {
				MessageRecieved(this, clientSocket, std::string(buffer, 0, bytesRecieved));
			}
		}
	} while (bytesRecieved > 0);
	closesocket(clientSocket);
}


void TcpListener::CleanUp() {

}

/**
 * a normal member using the Object parameters to Create a new SOCKET.
 * @param listeningSocket a SOCKET argument.
 * @return the listening SOCKET or -1 if failed
 */
SOCKET TcpListener::CreateSocket() {
	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listeningSocket == INVALID_SOCKET) {
		std::cerr << "cant create socket!";
		return -1;
	}

	//bind socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	//host to network short
	hint.sin_port = htons(this->m_port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	//Convert the ipAddress string into numeric binary form - IPV4 Format
	inet_pton(AF_INET, this->m_ipAddress.c_str(), &hint.sin_addr);

	int bindOk = bind(listeningSocket, (sockaddr*)&hint, sizeof(hint));
	if (bindOk == SOCKET_ERROR) {
		std::cerr << "cant bind socket!";
		return -1;
	}

	int listenOk = listen(listeningSocket, SOMAXCONN);
	if (listenOk == SOCKET_ERROR) {
		std::cerr << "socket cant listen!";
		return -1;
	}
	return listeningSocket;
}

/**
 * a normal member taking a listening SOCKET and returning a SOCKET.
 * @param listeningSocket a SOCKET argument.
 * @return the client SOCKET or -1 if failed
 */
SOCKET TcpListener::WaitForConnection(SOCKET listeningSocket, sockaddr_in* clientAddr) {
	int clientSize = sizeof(*clientAddr);
	SOCKET client = accept(listeningSocket, (sockaddr*)clientAddr, &clientSize);
	if (client == NULL) {
		std::cerr << "cant create client socket!";
		return -1;
	}
	return client;
}

/**
 * outputs the incoming connections in console
 * @param clientAddr a sockaddr_in argument.
 * @return void
 */
void TcpListener::VerboseConnection(sockaddr_in clientAddr) {
	char host[NI_MAXHOST]; //client's remote name
	char service[NI_MAXHOST]; //service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); //same as memset(host, 0, NI_MAXHOST); on unix
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&clientAddr, sizeof(clientAddr), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		std::cout << host << " connected on port " << service << std::endl;
	}
	else {
		inet_ntop(AF_INET, &clientAddr.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port " <<
			ntohs(clientAddr.sin_port) << std::endl;
	}
}
/**
 * outputs the disconnections in console
 * @param clientAddr a sockaddr_in argument.
 * @return void
 */
void TcpListener::VerboseDisconnection(sockaddr_in clientAddr) {
	struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&clientAddr;
	struct in_addr ipAddr = pV4Addr->sin_addr;
	char strIpAddr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipAddr, strIpAddr, INET_ADDRSTRLEN);

	std::cout << strIpAddr << " disconnected" << std::endl;
}