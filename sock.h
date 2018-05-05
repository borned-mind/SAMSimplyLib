#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include <unistd.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#define SOCK_CHECK(fals)\
	if(sock == 0){\
		 sock=m_main_sock;\
			if(m_main_sock == 0) return fals;\
	}

struct SockWrap{
	struct sockaddr_in addr;
	int sock;
};

class SamSock{
	protected:
			std::string m_host;
			int m_port;
		
			struct sockaddr_in m_main_sockaddr;
			int m_main_sock;
			virtual void close_connection(void) = 0;
	public:
		SamSock(void){}
		SamSock(std::string host, int port) :m_host(host),m_port(port),m_main_sock(0) {}
		virtual SockWrap getsock(std::string host, int port) =0;
		virtual int setsock(SockWrap sock) =0;
		int & getSock(void) { return m_main_sock; } // for setsockopt...getsockopt...write..read not via below


		bool wrt(const std::string msg, int sock=0);
		bool wrt(const void * buf, const ssize_t n, int sock=0); 
		bool wrt(int sock, const void * buf, const ssize_t n); 

		std::string reading(std::size_t count) noexcept;
		std::string reading(int sock, std::size_t count) noexcept;

		int connecting_to_sam(std::string host, int port);
		
};
