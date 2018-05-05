#pragma once
#include<sstream>

#include<map>
#include"util.h"
using list_args = std::map<std::string, std::string >  ;
namespace Util{
	void msg_join(std::string msg) noexcept{}

	template<typename Tomsg, typename ... Tmsgs> void msg_join(std::string & msg, Tomsg omsg, Tmsgs ... msgs) noexcept{
			msg += static_cast<std::ostringstream*>( &(std::ostringstream() << omsg) )->str();
			msg_join(msg, msgs...);
	}

	template<typename ... Tmsgs > void throw_error(std::string msg, Tmsgs ... msgs){
			msg_join(msg, msgs...);
			throw( std::runtime_error(msg) );
	}

	list_args split(std::string where, char d=' '){
		std::cout << "Spliting string: " << where << std::endl;
		std::istringstream tmp(where) ;
		std::string element;
		


		list_args retur;
		while( std::getline(tmp, element, d) ){
			auto eq = element.find("=");
			if( eq == std::string::npos ) {
				retur[element]="";
				continue;
			}else{
				retur[ element.substr(0,eq) ] = element.substr(eq+1);
				continue;
			}
		}

		return retur;

	}

	bool exists(std::map<std::string, std::string> mp, std::string what){
		try{
			mp.at(what);
			return true;
		}catch(...){return false;}
	}
}
