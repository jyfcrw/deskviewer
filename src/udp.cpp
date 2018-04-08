#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp> //ʹ���ַ���ת������  

using namespace std;
using namespace boost::asio;

#define PORT 9999

#ifdef _MSC_VER
#define _WIN32_WINNT    0X0501  //����VC�±��뾯��  
#endif  

int udp_server()
{
	io_service ios;
	ip::udp::endpoint server_ep(ip::udp::v4(), PORT);
	ip::udp::socket sock(ios, server_ep);
	while (true)
	{
		char buf[1024];
		boost::system::error_code ec;
		ip::udp::endpoint remote_ep;//����Զ�����ӽ����Ķ˵�  
		sock.receive_from(buffer(buf), remote_ep, 0, ec);
		if (ec && ec != error::message_size)
		{
			throw boost::system::system_error(ec);
		}
		cout << "receive from remote ip:" << remote_ep.address() << endl;
		cout << "receive from remote port:" << remote_ep.port() << endl;
		sock.send_to(buffer("I heard you!"), remote_ep);
	}
	return 0;
}

int udp_client()
{
	io_service ios;
	//Ҫ�����ķ���˵�  
	//udp::resolver resolver(service); // #2
	//udp::resolver::query query(udp::v4(), "localhost", "8868"); // #3
	//udp::endpoint receiverEndpoint = *resolver.resolve(query); // #4

	ip::udp::endpoint send_ep(ip::address::from_string("127.0.0.1"), PORT);
	ip::udp::socket sock(ios);
	sock.open(ip::udp::v4());
	//�����˷�������  
	sock.send_to(buffer("hello"), send_ep);

	//��������  
	vector<char> v(1024, 0);
	ip::udp::endpoint ep;
	sock.receive_from(buffer(v), ep);
	cout << "receive from ip:" << ep.address() << endl;
	cout << "receive msg:" << &v[0] << endl;
}