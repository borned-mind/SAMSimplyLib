#include<map>
#include"sock.h"
#include <utility>
#include<memory>
#include<string.h>



#define ELIF(any) else if(any)




namespace SAM{
	constexpr std::size_t defDataBlock=2048;

	//using keys_t = //std::pair<std::string,std::string>; // public, private
//	enum class type{	};


	using command_value=std::pair<std::string, std::string>;
	
	


	struct keys_t{ 
		public:
			keys_t(std::string pub, std::string priv):pub(pub),priv(priv){}
			keys_t(void):pub("DELETED"),priv("DELETED"){}
			std::string pub;
			std::string priv;
	};
/*
	enum class type_conn{
		connect, accept, other
	};
*/

	struct SAMConnection : public SamSock {
		int setsock(SockWrap)   override{}
		SockWrap getsock(std::string, int) override{}
		void close_connection() override { close (m_main_sock); }
/*
		union{
			int conn;
			int accept;
			int weak;
		}m_sock;
*/
		

		public:
			//const decltype(auto) getBuf(void){return (buf); }
			SAMConnection operator=(SAMConnection sam){
				this->m_main_sock=sam.m_main_sock;
				this->m_main_sockaddr=sam.m_main_sockaddr;
			}
			auto getSock(void) -> int{

				return this->m_main_sock;
			}
			SAMConnection( SockWrap sock );
	};	


	
	constexpr char defHost[] = "localhost";
	constexpr int defPort=7656;
	


	class SAM : public SamSock{
		private:
			std::string last_result;
			//bool session_exist=false;
			 inline bool ifner(std::map<std::string, std::string>  answ);
			 inline bool send_and_wait(std::string NameConn, std::string comm,  std::initializer_list<command_value> l={});
		protected:
			//keys_t m_keys;

		protected:

			int setsock(SockWrap)  noexcept override;
			SockWrap getsock(std::string, int) override;

			void close_connection(void)  noexcept override;

			bool isSAM( void ) ;
			bool isSAM( std::string host, int port );
			

		public:
			bool isSAM(int sock);
		public:
			std::map<std::string, keys_t> other_keys;
			std::map<std::string, std::shared_ptr<SAMConnection> > connections;
			std::map<std::string, keys_t> sessions_dests;
		public:
			const std::string & getLastResult(void){return last_result;}
			std::map<std::string, std::string > operator()(std::string comm,  std::initializer_list<command_value> l={}, int sock=0){
				return send_command(comm, l, sock);
			}
			std::map<std::string, std::string >
				 send_command(std::string comm,  std::initializer_list<command_value> l={}, int sock=0);


			keys_t generate_keys(void) ;
			//void set_keys(keys_t keys) noexcept;
			void addAnotherKey(std::string name, keys_t key);


			bool reconnect(void);
			

			bool connect_to(std::string ID, std::string dest, std::string NameConn);
			bool accepting(std::string ID, std::string NameConn);

			SAM(std::string host=defHost,
				 int port=defPort,
					  bool check_is_sam=true, bool con=true );
			~SAM(void);
			
		public:
			//keys_t & getKeys(void) noexcept { return m_keys; }
		public:
			bool init_session(std::string ID, std::string type="STREAM");



	};
	

};
