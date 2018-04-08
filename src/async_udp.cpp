#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace std;
using boost::asio::ip::udp;


class Server {
public:
	Server(boost::asio::io_service& service) :
		sock(service, udp::endpoint(udp::v4(), 8868)) {
		start();
	}

private:
	void start() {
		memset(buf, 0, BUF_SIZE);
		sock.async_receive_from(boost::asio::buffer(buf), remoteEndpoint,
			boost::bind(&Server::handleReceive, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
	void handleReceive(const boost::system::error_code& error,
		std::size_t bytes_transferred) {
		if (!error || error == boost::asio::error::message_size) {
			cout << remoteEndpoint << endl;
			sock.async_send_to(boost::asio::buffer(buf, bytes_transferred),
				remoteEndpoint, boost::bind(&Server::handleSend, this,
					boost::asio::placeholders::error));
			start();
		}
	}
	void handleSend(const boost::system::error_code& /*error*/) {
	}
private:
	udp::socket sock;
	udp::endpoint remoteEndpoint;

	enum {
		BUF_SIZE = 512
	};
	char buf[BUF_SIZE];
};


class Client {
public:
	Client(boost::asio::io_service& service, const udp::endpoint& remote) :
		remoteEndpoint(remote), sock(service, udp::v4()) {
		// sock.open(udp::v4());
		start();
	}

private:
	void start() {
		memset(buf, 0, BUF_SIZE);
		cin.getline(buf, BUF_SIZE);
		sock.async_send_to(boost::asio::buffer(buf, strlen(buf)),
			remoteEndpoint, boost::bind(&Client::handleSend, this,
				boost::asio::placeholders::error));
	}
	void handleSend(const boost::system::error_code& error) {
		if (!error) {
			memset(buf, 0, BUF_SIZE);
			udp::endpoint local;
			sock.async_receive_from(boost::asio::buffer(buf, BUF_SIZE), local,
				boost::bind(&Client::handleReceive, this,
					boost::asio::placeholders::error));
		}
	}
	void handleReceive(const boost::system::error_code& error) {
		if (!error) {
			cout << buf << endl;
			start();
		}
	}
private:
	udp::endpoint remoteEndpoint;
	udp::socket sock;
	enum {
		BUF_SIZE = 512
	};
	char buf[BUF_SIZE];
};

void start() {
	// server
	try {
		boost::asio::io_service service;
		Server server(service);
		service.run(); // 注意：一定要调用 run()函数
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	
	// client
	try {
		boost::asio::io_service service;
		udp::resolver resolver(service);
		udp::resolver::query query(udp::v4(), "localhost", "8868");
		udp::endpoint receiverEndpoint = *resolver.resolve(query);

		cout << receiverEndpoint << endl;
		//        cout << receiverEndpoint.address().to_string() << endl;
		//        cout << receiverEndpoint.port() << endl;
		Client c(service, receiverEndpoint);

		service.run();
	}
	catch (exception& e) {
		cout << e.what() << endl;
	}


}
