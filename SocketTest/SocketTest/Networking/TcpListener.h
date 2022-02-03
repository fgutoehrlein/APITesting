#pragma once

#include <string>
#include <iostream>
#include "HttpResponder.h"

#ifdef __unix__         
#ifndef OS_Linux
#define OS_Linux
#endif

#elif defined(_WIN32) || defined(WIN32) 
#ifndef OS_Windows
# define OS_Windows
#include <ws2tcpip.h>				//header for Winsock functions
#pragma comment(lib, "ws2_32.lib")	//winsock library file
#endif
#endif

#define MAX_SOCKET_BUFFER_SIZE (49152)

//Forward declaration of class
class TcpListener;

//Callback to data recieved
typedef void (*MessageRecievedHandler)(TcpListener* listener, int socketId, std::string msg);

/**
* A class which handles a socket and its connection i.e. a Server
 */
class TcpListener {

public:
	TcpListener(std::string ipAddress, int port, MessageRecievedHandler handler, bool verbose = false);

	~TcpListener();

	/**
	 * a normal member using the client SOCKET and a message to send it.
	 * @param clientSocket a SOCKET argument.
	 * @param message a std::string argument.
	 * @return the listening SOCKET or -1 if failed
	 */
	void Send(int clientSocket, std::string message);

	//Initialize winsock
	bool Init();

	//The main processing loop

	void Run();

	void HandleClientSocket(const SOCKET& clientSocket, const sockaddr_in& clientAddr, const SOCKET& listeningSocket, char  buffer[49152]);

	void CleanUp();
private:
	std::string				m_ipAddress;
	int						m_port;
	MessageRecievedHandler	MessageRecieved;
	bool					Verbose;

	/**
	 * a normal member using the Object parameters to Create a new SOCKET.
	 * @param listeningSocket a SOCKET argument.
	 * @return the listening SOCKET or -1 if failed
	 */
	SOCKET CreateSocket();

	/**
	 * a normal member taking a listening SOCKET and returning a SOCKET.
	 * @param listeningSocket a SOCKET argument.
	 * @return the client SOCKET or -1 if failed
	 */
	SOCKET WaitForConnection(SOCKET listeningSocket, sockaddr_in* clientAddr);
	/**
	 * outputs the incoming connections in console
	 * @param clientAddr a sockaddr_in argument.
	 * @return void
	 */
	void VerboseConnection(sockaddr_in clientAddr);
	/**
	 * outputs the disconnections in console
	 * @param clientAddr a sockaddr_in argument.
	 * @return void
	 */
	void VerboseDisconnection(sockaddr_in clientAddr);
};