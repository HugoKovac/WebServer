#include "includes.hpp"

char **Server::cgi_vars(struct header & header, id_server_type server_id, std::string php_arg, std::string method){
	std::map<std::string, std::string> line;

	line["REQUEST_METHOD"] = method;
	line["AUTH_TYPE"] = "";
	line["CONTENT_LENGTH"] = "0";
	line["GATEWAY_INTERFACE"] = "CGI/1.1";
	line["REDIRECT_STATUS"] = "";
	if (method == "GET")
		line["QUERY_STRING"] = confs[server_id].query_string ;
	line["SERVER_PROTOCOL"] = "HTTP/1.1";
	line["SERVER_SOFTWARE"] = "WebServ/1.0";
	line["SCRIPT_NAME"] = php_arg;//parse php_arg
	line["SCRIPT_FILENAME"] = php_arg;
	line["PATH_INFO"] = php_arg;
	if (confs[server_id].server_names.size() > 0)
		line["SERVER_NAME"] = confs[server_id].server_names[0];
	else
		line["SERVER_NAME"] = "webserv";
	line["SERVER_PORT"] = confs[server_id].listening.begin()->second[0];
	line["PATH_TRANSLATED"] = php_arg;
	line["REQUEST_URI"] = php_arg + confs[server_id].query_string;
	line["REMOTE_ADDR"] = "";
	line["REMOTE_USER"] = "";
	line["REMOTE_HOST"] = "";
	line["REMOTE_IDENT"] = "";
	if (method == "POST"){
		line["CONTENT_TYPE"] = header.raw_content_type;
		line["CONTENT_LENGTH"] = SSTR(header.content_length);
	}

	
	char	**env = new char*[line.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = line.begin(); i != line.end(); i++) {
		std::string	element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	return env;
}

std::string Server::parseCgiHeader(std::string buffer){
	HGen.clear();

	if (buffer.find("\r\n\r\n") == std::string::npos)
		return buffer;

	std::string rtn;
	std::string header;
	std::string CT = "Content-type: ";
	std::string S = "Status: ";
	std::string LOC = "Location: ";
	std::string::size_type st;
	
	header = buffer.substr(0, buffer.find("\r\n\r\n"));
	rtn = buffer.substr(header.size() + 4, buffer.size());

	if ( (st = header.find(CT)) != std::string::npos )
		HGen.setType(header.substr(st + CT.size(), header.find_first_of("\r", st) - CT.size()));

	if ( (st = header.find(S)) != std::string::npos )
		HGen.setStatus(header.substr(st + S.size(), header.find_first_of("\r", st) - S.size()));

	if ( (st = header.find(LOC)) != std::string::npos ){
		HGen.setLocation(header.substr(st + LOC.size(), header.find_first_of("\r", st) - LOC.size()));
		HGen.processResponse();
	}

	// std::clog << HGen.getStr() << std::endl;
	return rtn;
}

bool Server::cgi_error(id_server_type server_id){
	// std::clog << "*" << HGen.getStatus() << "*" << std::endl;
	if (HGen.getStatus().find("200") != std::string::npos || HGen.checkStatus())
		return false;
	if (HGen.getStatus().find("500") != std::string::npos)
		send_500(server_id);
	else if (HGen.getStatus().find("400") != std::string::npos)
		send_400(server_id);
	else if (HGen.getStatus().find("404") != std::string::npos)
		send_404(server_id);

	return true;
}


void Server::php_cgi(struct header & header, id_server_type server_id, std::string php_arg, std::string method){
	char **env = cgi_vars(header, server_id, php_arg, method);
	std::string buffer_cout = cgi_exec(header, server_id, php_arg, method, env);
	// std::clog << "php_arg : "<< php_arg << buffer_cout << std::endl;

	send_cgi(server_id, parseCgiHeader(buffer_cout));

	// std::clog << "=========================" << std::endl;
	for (size_t i = 0; env[i]; i++){
		// std::clog << env[i] << std::endl;
		delete[] env[i];
	}
	delete[] env;
}