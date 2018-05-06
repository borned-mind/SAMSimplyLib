#include"sam.h"

int main(void){

	auto s = SAM::SAM();
	//auto keys = s("DEST GENERATE");
	//std::cout << "PUBLIC: " << keys["PUB"] << " PRIVATE: " << keys["PRIV"] << std::endl;
	//s.send_command("HELLO VERSION", { {"MIN","2"}, {"MAX","5"} } );
	//s("HELLO VERSION", { {} });



	
	try{

		if(! s.init_session("TestingSession27") ) std::cerr << "Can't init session" << std::endl;
		if( ! s.accepting("TestingSession27", "Acepting_conn") ) std::cerr << "Can't accept" << std::endl;
		auto ses = *( s.connections["Acepting_conn"].get() );
		std::cout << "To cycle" << std::endl;
		while(1){
			std::cout << "Get buf" << std::endl;
			
			auto buf = ses.reading(1024);
			std::cout << "Write buf" << std::endl;
			std::cout <<  buf << std::endl;
			ses.wrt("PONG: "+buf);
		}


	}catch(std::runtime_error & e){
			std::cerr << "Error: " << e.what() << std::endl;
	}


}
