#include "AsioSocketTest.h"

using namespace boost;

int main()
{
	// The size of the queue containing the pending connection
	// requests.
	const int BACKLOG_SIZE = 30;

	// Step 1. Here we assume that the server application has
	// already obtained the protocol port number.
	unsigned short port_num = 54010;

	// Step 2. Creating a server endpoint.
	auto ipAddr = asio::ip::address_v4::from_string("127.0.0.1");
	asio::ip::tcp::endpoint ep(ipAddr, port_num);

	asio::io_service ios;

	try {
		// Step 3. Instantiating and opening an acceptor socket.
		asio::ip::tcp::acceptor acceptor(ios, ep.protocol());

		// Step 4. Binding the acceptor socket to the 
		// server endpint.
		acceptor.bind(ep);

		// Step 5. Starting to listen for incoming connection
		// requests.
		acceptor.listen(BACKLOG_SIZE);

		// Step 6. Creating an active socket.
		asio::ip::tcp::socket sock(ios);

		// Step 7. Processing the next connection request and 
		// connecting the active socket to the client.

		acceptor.accept(sock);

		// At this point 'sock' socket is connected to 
		// the client application and can be used to send data to
		// or receive data from it.

		while (sock.is_open()) {
			boost::asio::streambuf buf;
			boost::asio::read_until(sock, buf, "\n");
			std::string data = boost::asio::buffer_cast<const char*>(buf.data());

			std::cout << data;

			std::string message = "hello world";
			boost::asio::write(sock, boost::asio::buffer(message));
		}
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}