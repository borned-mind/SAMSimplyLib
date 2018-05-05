#include"sock.h"


bool SamSock::wrt(int sock, const void * buf, const ssize_t n){
	SOCK_CHECK(false);
	return write(sock, buf, n) == n;
}


bool SamSock::wrt(const void * buf, const ssize_t n, int sock){
	SOCK_CHECK(false);
	return wrt(sock, buf, n);
	
}


bool SamSock::wrt(const std::string msg, int sock){
	SOCK_CHECK(false);
	return wrt(msg.c_str(), msg.size(), sock);
}

		

		std::string reading(std::size_t count) noexcept;
		std::string reading(int sock, std::size_t count) noexcept;


std::string SamSock::reading(int sock, std::size_t count) noexcept{
	SOCK_CHECK("");

	char * buf = new char[count]; //calloc(count); is not simillar to malloc as i see; or create in stack char buf[count];
	read(sock, buf, count);
	std::string ret = buf;
	delete [] buf;
	return ret;
}

std::string SamSock::reading(std::size_t count) noexcept{
	reading(m_main_sock, count);
}


