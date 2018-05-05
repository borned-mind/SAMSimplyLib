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

	char * buf = (char*)calloc(count, 1); 
	read(sock, buf, count);
	std::string ret = std::string(buf);
	std::cout << "Buf is: " << buf << std::endl;
	delete [] buf;
	std::cout << "Ret is: " << ret << std::endl;
	return ret;
}

std::string SamSock::reading(std::size_t count) noexcept{
	reading(m_main_sock, count);
}


