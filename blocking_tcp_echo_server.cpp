#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
using namespace boost::system;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;
using namespace boost::asio::detail;
//using asio::ip::tcp;
const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

void session(socket_ptr sock)
{
  try
  {
    for (;;)
    {
      char data[max_length];

      error_code error;
      size_t length = sock->read_some(buffer(data), error);
      if (error == error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw system_error(error); // Some other error.

      write(*sock, buffer(data, length));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(io_context& io_context, unsigned short port)
{
  tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    socket_ptr sock(new tcp::socket(io_context));
    a.accept(*sock);
	cout<<*sock<<" on line"<<endl;
    thread t(boost::bind(session, sock));
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
      return 1;
    }

    io_context io_context;

    using namespace std; // For atoi.
    server(io_context, atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

