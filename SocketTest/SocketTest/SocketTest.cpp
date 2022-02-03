// SocketTestCmake.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "SocketTest.h"

using namespace std;

void Listener_MessageRecieved(TcpListener* listener, int client, string msg);

int main()
{
	TcpListener server("127.0.0.1", 54010, Listener_MessageRecieved);

	if (server.Init()) {
		server.Run();
	}

	return 0;
}

void Listener_MessageRecieved(TcpListener* listener, int client, string msg) {
	listener->Send(client, msg);
}
