#include"sam.h"
#include"util.h"
namespace SAM{

	inline bool SAM::ifner(std::map<std::string, std::string>  answ){

				if( !Util::exists(answ, "RESULT") ) return false;

				last_result=answ["RESULT"];


				if(answ["RESULT"] != "OK") 
					Util::throw_error("ERROR: Result is not ok, is "+answ["RESULT"]);

				return true;

	}

	SAM::SAM(std::string host, int port, keys_t keys, bool check_is_sam)
	:SamSock(host,port), m_keys(keys){
		try{
			setsock( getsock(m_host, m_port) );
			if(check_is_sam)
				if( !isSAM() ) throw( std::runtime_error("Is not SAM?") );

		}catch(std::runtime_error & e){
			std::cerr << "ERROR: " << e.what() << std::endl;
			//delete this;
		}	
	}



	list_args SAM::send_command(std::string  comm,  std::initializer_list<command_value> values, int sock){
			
			for(auto val : values)
				Util::msg_join( comm, " ", std::get<0>(val),"=",std::get<1>(val) );
			std::cout << "I will send command: " << comm << std::endl;
			wrt(comm+'\n', sock);
			std::cout << "Now read" << std::endl;
			return Util::split( reading(sock, defDataBlock) );	
	}


	bool SAM::init_session(std::string ID, std::string type){
			if(!m_keys.priv.size()) return false;
			auto answ = send_command("SESSION CREATE", {
								{"STYLE",type},
								{"ID",ID},
								{"DESTINATION",m_keys.priv}
									 } );

			if(!ifner(answ)) return false;
			sessions_dests[ID]=answ["DESTINATION"];
			return true;
	}
	

	bool SAM::connect_to(std::string dest, std::string ID, std::string NameConn){

			auto tmp_sam=	std::make_shared<SAMConnection
			>(SAMConnection(type_conn::connect, getsock(m_host, m_port)));

			auto answ = send_command("STREAM CONNECT", { {"ID",ID},{"DESTINATION",dest} },
				(tmp_sam.get())->getSock());
			(tmp_sam.get())->update_buf();
			connections[NameConn]=tmp_sam;

			return ifner(answ);
			
	}



	keys_t SAM::generate_keys(/*std::string signaturetype*/) {
	
		auto keys = send_command("DEST GENERATE");//{ {"SIGNATURE_TYPE", signaturetype} } );
		if( !Util::exists(keys, "PUB") || !Util::exists(keys, "PRIV")  ) Util::throw_error("Can't gen key");
		
		return {keys["PUB"],keys["PRIV"]};
	}


	void SAM::addAnotherKey(std::string name, keys_t key){
		other_keys[name]=key;
	}

	

	void SAM::set_keys(keys_t keys) noexcept{
		m_keys=keys;
	}


	void SAM::close_connection(void) noexcept{
		wrt("EXIT\n");
		close(m_main_sock);
	}

	SAM::~SAM(void){
		close_connection();
		m_keys={};
	}


	bool SAM::isSAM(void) {

		auto list = send_command( "HELLO VERSION" );
		//std::cout << list["RESULT"] << std::endl;
		if( !ifner(list) ) return false;
		return true;
	


	//	for( auto elm : list )
	//		std::cout << elm.first << " =is= " << elm.second << std::endl; 
	
			//if(elm == "RESULT=OK") return true;
			
	}

	bool SAM::isSAM(std::string host, int port){
		close_connection();
		m_host=host;
		m_port=port;
		return reconnect();
	}

	bool SAM::reconnect(void){
		try{
			wrt("QUIT");
			close_connection();
			setsock( getsock(m_host, m_port) );
			return true;
		}catch(...){return false;}
	}


			
	SockWrap SAM::getsock(std::string hostaddr, int port){
		int tmp_sock;
		struct sockaddr_in tmpsockaddr;

		tmp_sock=socket(AF_INET, SOCK_STREAM, 6);
		if(tmp_sock == -1) Util::throw_error("Can't create socket");

		

		struct hostent * host=gethostbyname(hostaddr.c_str());
		memcpy(&tmpsockaddr.sin_addr,
				 host->h_addr_list[0], 
					host->h_length);

		tmpsockaddr.sin_family=AF_INET;
		tmpsockaddr.sin_port=htons(port);
		//m_main_sockaddr.sin_addr.s_addr=inet_addr(m_host.c_str());
		
		if( connect(tmp_sock, (struct sockaddr*)&tmpsockaddr, sizeof tmpsockaddr) == -1)
			Util::throw_error("Can't connect to SAM on ", hostaddr, ":", port);
		return {tmpsockaddr, tmp_sock};
	}

	int SAM::setsock(SockWrap sock) noexcept {
		m_main_sock=sock.sock;
		m_main_sockaddr=sock.addr;
		
	}

	SAMConnection::SAMConnection(type_conn typ, SockWrap sock){

			switch(typ){
				case type_conn::connect:
					m_sock.conn=sock.sock;
					break;
				case type_conn::accept:
					m_sock.accept=sock.sock;
					break;
				case type_conn::other:
				default:
					m_sock.weak=sock.sock;
					break;
			}
	}

	SAMConnection::~SAMConnection(void){
		if(m_sock.conn != 0) close(m_sock.conn);
		ELIF(m_sock.accept != 0) close(m_sock.accept);
		ELIF(m_sock.weak != 0) close(m_sock.weak);
	}

};