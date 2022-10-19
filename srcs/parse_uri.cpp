#include "includes.hpp"


/*parse value/key in uri and return uri without this values*/
std::string Server::parse_uri(struct header & header, id_server_type server_id){
	typedef std::string::size_type s_t;
	
	std::string uri = header.path;
	s_t pos_i;
	std::string rtn;


	pos_i = uri.find("?");
	if (pos_i == std::string::npos)
		return header.path;

	rtn = header.path.substr(0, pos_i);

	confs[server_id].query_string = header.path.substr(pos_i + 1, header.path.size() - 1);

	// s_t pos_end;

	// /**
	//  * @brief quoi faire si
	//  * ?value=
	//  * ?=key
	//  * ?=
	//  * ?value
	//  */
	// while ((pos_end = header.path.find("&", pos_i + 1)) != std::string::npos){
	// 	std::string exp = header.path.substr(pos_i + 1, pos_end - (pos_i + 1));

	// 	confs[server_id].get_vars[exp.substr(0, exp.find("="))] = exp.substr(exp.find("=") + 1, exp.size() - 1);

	// 	pos_i = pos_end;
	// }

	// std::string exp = header.path.substr(pos_i + 1, header.path.size() - 1);
	// confs[server_id].get_vars[exp.substr(0, exp.find("="))] = exp.substr(exp.find("=") + 1, exp.size() - 1);


	// {
	// 	std::cout << "key = " << it->first << std::endl;
	// 	std::cout << "value = " << it->second << std::endl;
	// }



	return rtn;
}