#include"sam.h"
#include"util.h"
namespace SAM{

	inline bool SAM::ifner(std::map<std::string, std::string>  answ){
				

				if( !Util::exists(answ, "RESULT") ) return false;

				
				last_result=answ["RESULT"];

			
				if(answ["RESULT"] != "OK" && answ["RESULT"] != "OK\n") 
					Util::throw_error("ERROR: Result is not ok, is "+answ["RESULT"]);

				return true;

	}

	SAM::SAM(std::string host, int port, bool check_is_sam, bool con)
	:SamSock(host,port){
		try{
			if(con)
				setsock( getsock(m_host, m_port) );
			if(check_is_sam)
				if( !isSAM() ) throw( std::runtime_error("Is not SAM?") );

		}catch(std::runtime_error & e){
			std::cerr << "ERROR: " << e.what() << std::endl;
			//delete this;
		}	
	}



	list_args SAM::send_command(std::string  comm,  std::initializer_list<command_value> values, int sock){
			sock = (sock == 0) ? m_main_sock : sock;
			for(auto val : values)
				Util::msg_join( comm, " ", std::get<0>(val),"=",std::get<1>(val) );

			wrt(comm+'\n', sock);

			return Util::split( reading( sock, defDataBlock) );	
	}


	bool SAM::init_session(std::string ID, std::string type){
			sessions_dests[ID]=generate_keys();
			

			
			auto answ = send_command("SESSION CREATE", {
								{"STYLE",type},
								{"ID",ID},
								{"DESTINATION", sessions_dests[ID].priv }
									 } );
			
			if(!ifner(answ)) return false;
			std::cout << "New session ID=" << ID << " was created " << std::endl
			<< "PUB= " << sessions_dests[ID].pub << std::endl 
			<< "PRIV= " << sessions_dests[ID].priv << std::endl;


			return true;
	}
	
	inline bool SAM::send_and_wait(std::string NameConn, std::string comm,  std::initializer_list<command_value> l){
			

			auto tmp_sock = getsock(m_host, m_port);
			connections[NameConn]=	std::make_shared<SAMConnection
			>( SAMConnection(tmp_sock) );



			auto answ = send_command(comm, l,
				(connections[NameConn].get())->getSock()	);

		

			return ifner(answ);	
	}
	
	bool SAM::connect_to(std::string ID, std::string dest, std::string NameConn){
			return send_and_wait(NameConn, "STREAM CONNECT", { {"ID",ID},{"DESTINATION",dest}});
	}

	bool SAM::accepting(std::string ID, std::string NameConn){
			return send_and_wait(NameConn, "STREAM ACCEPT", { {"ID",ID}});
	}

	keys_t SAM::generate_keys(/*std::string signaturetype*/) {
	
		auto keys = send_command("DEST GENERATE");//{ {"SIGNATURE_TYPE", signaturetype} } );
		if( !Util::exists(keys, "PUB") || !Util::exists(keys, "PRIV")  ) Util::throw_error("Can't gen key");
		
		return {keys["PUB"],keys["PRIV"]};
	}


	void SAM::addAnotherKey(std::string name, keys_t key){
		other_keys[name]=key;
	}

	

	/*void SAM::set_keys(keys_t keys) noexcept{
		m_keys=keys;
	}*/


	void SAM::close_connection(void) noexcept{
		wrt("EXIT\n");
	
		close(m_main_sock);
	}

	SAM::~SAM(void){
		close_connection();
		for( auto conn : connections ){
			(conn.second.get())->wrt("EXIT\n");
			(conn.second.get())->close_connection();
		}

		//m_keys={};
	}

	bool SAM::isSAM(int sock) {
				auto list = send_command( "HELLO VERSION", {}, sock );
				//for(auto l : list)std::cout << l.first << "=" << l.second << std::endl;
				
				if( !ifner(list) ) return false;
				return true;
	}

	bool SAM::isSAM(void) {

		return isSAM(m_main_sock);
	


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

	SAMConnection::SAMConnection(SockWrap sock){
			m_main_sock=sock.sock;
			m_main_sockaddr=sock.addr;

			auto t=SAM("",0,false, false);
			if( !t.isSAM(m_main_sock) ) 
				Util::throw_error("#Is not SAM");

			
	}

	

};
