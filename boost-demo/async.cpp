#include <boost/asio.hpp>
#include <iostream>

void print(const boost::system::error_code& ec){
	std::cout << "Hello World!" << std::endl;
}

int main() {
	boost::asio::io_context io;
	boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

	t.async_wait(&print);

	io.run();

	return 0;

}
