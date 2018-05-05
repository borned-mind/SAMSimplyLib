#include"sam.h"

int main(void){

	auto s = SAM::SAM();
	//auto keys = s("DEST GENERATE");
	//std::cout << "PUBLIC: " << keys["PUB"] << " PRIVATE: " << keys["PRIV"] << std::endl;
	//s.send_command("HELLO VERSION", { {"MIN","2"}, {"MAX","5"} } );
	//s("HELLO VERSION", { {} });



	
	try{
		//s.set_keys(s.generate_keys());
		if(! s.init_session("TestingSession23") ) std::cerr << "Can't init session" << std::endl;
		//if(! s.init_session("TestingSession23Conn") ) std::cerr << "Can't init session" << std::endl;
	
		if( ! s.accepting("TestingSession23", "Acepting_conn") ) std::cerr << "Can't accept" << std::endl;
		while(1){
			std::cout << "Get buf" << std::endl;
			(s.connections["TestingSession23"].get() )->update_buf();
			auto buf = (s.connections["TestingSession23"].get() ) -> getBuf();
			std::cout << "Buf is: " << buf << std::endl;
		}
	//	s.connect_to("TestingSession23Conn",  s.sessions_dests["TestingSession23"].pub, "Connect_conn");
	// 	(s.connections["Connect_conn"].get()) -> wrt( std::string("Test?"), s.getSock());
	//	auto buf = (s.connections["TestingSession23Conn"].get() ) -> getBuf();
		//std::cout << "Buf is: " << buf << std::endl;

	}catch(std::runtime_error & e){
			std::cerr << "Error: " << e.what() << std::endl;
	}


}
