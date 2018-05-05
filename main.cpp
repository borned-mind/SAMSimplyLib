#include"sam.h"

int main(void){

	auto s = SAM::SAM();
	//auto keys = s("DEST GENERATE");
	//std::cout << "PUBLIC: " << keys["PUB"] << " PRIVATE: " << keys["PRIV"] << std::endl;
	//s.send_command("HELLO VERSION", { {"MIN","2"}, {"MAX","5"} } );
	//s("HELLO VERSION", { {} });



	
	try{
		s.set_keys(s.generate_keys());
		if(! s.init_session("TestingSession") ) std::cerr << "Can't init session" << std::endl;
	}catch(std::runtime_error & e){
			std::cerr << "Error: " << e.what() << std::endl;
	}


}
