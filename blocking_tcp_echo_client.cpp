#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

//using asio::ip::tcp;
enum { max_length = 1024 };

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
      return 1;
    }

    io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
      resolver.resolve(tcp::v4(), argv[1], argv[2]);

    tcp::socket s(io_context);
    connect(s, endpoints);

    using namespace std; // For strlen.
    std::cout << "Enter message: ";
    char request[max_length];
	char buff[max_length];
    std::cin.getline(request, max_length);
	sprintf(buff,"%s-%s",ip::host_name().c_str(),request);
    size_t request_length = strlen(buff);
    write(s, buffer(buff, request_length));

    char reply[max_length];
    size_t reply_length = read(s,
        buffer(reply, request_length));
	s.close();
    std::cout << "Reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

