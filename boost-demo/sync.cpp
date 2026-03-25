#include <boost/asio.hpp>
#include <iostream>

int main() {
	boost::asio::io_context io;
	boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
	boost::system::error_code ec;

	t.wait(ec);

	std::cout << "Hello World!" << std::endl;

	return 0;


}
