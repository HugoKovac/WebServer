#include "includes.hpp"

void Server::send_responses(std::string file){ //? Look for the file extension and send the proper content type
	std::string content_type = return_content_type(file);

	send_200(file, content_type);
}

void Server::send_204( void )
{
	HGen.clear();

	HGen.setStatus("204 No Content");
	HGen.setType("text/html");
	HGen.processResponse();
}

void Server::send_201( void )
{
	HGen.clear();

	HGen.setStatus("201 Created");
	HGen.setType("text/html");
	HGen.processResponse();
}

void Server::send_200_chunked( void )
{
	HGen.clear();

	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setTransEnc("chunked");
	HGen.processResponse();
}

void Server::send_200_autoindex(std::string content){
	HGen.clear();
	HGen.setStatus("200 OK");
	HGen.setType("text/html");
	HGen.setContentString(content);
	HGen.processResponse();
}

void Server::send_200( std::string file, std::string & extension ){
	HGen.clear();
	std::string fileSTR = fileToString(file);

	HGen.setStatus("200 OK");
	HGen.setType(extension);
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_202(void){
	HGen.clear();

	HGen.setStatus("202 Accepted");
	HGen.setType("text/html");
	HGen.processResponse();
}

void Server::send_400(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR;

	if ( confs[serverNb].errors[400].empty() == false)
		fileSTR = get_error_page(confs[serverNb].errors[400]);
	else
		fileSTR = generate_400();

	HGen.setStatus("400 Bad Request");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_413(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR;

	if ( confs[serverNb].errors[413].empty() == false)
		fileSTR = get_error_page(confs[serverNb].errors[413]);
	else
		fileSTR = generate_413();

	HGen.setStatus("413 Bad Request");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_404(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR;

	if ( confs[serverNb].errors[404].empty() == false)
		fileSTR = get_error_page(confs[serverNb].errors[404]);
	else
		fileSTR = generate_404();

	HGen.setStatus("404 Not Found");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_405(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR;

	if ( confs[serverNb].errors[405].empty() == false )
		fileSTR = get_error_page(confs[serverNb].errors[405]);
	else
		fileSTR = generate_405();

	HGen.setStatus("405 Method Not Allowed");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_414( id_server_type serverNb ){
	HGen.clear();
	std::string fileSTR;

	if ( confs[serverNb].errors[414].empty() == false )
		fileSTR = get_error_page(confs[serverNb].errors[414]);
	else
		fileSTR = generate_414();

	HGen.setStatus("414 URI Too Long");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_500(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR;

	if ( confs[serverNb].errors[500].empty() == false)
		fileSTR = get_error_page(confs[serverNb].errors[500]);
	else
		fileSTR = generate_500();

	HGen.setStatus("500 Internal Server Error");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_501(id_server_type serverNb){
	HGen.clear();
	std::string fileSTR;

	if ( confs[serverNb].errors[501].empty() == false)
		fileSTR = get_error_page(confs[serverNb].errors[501]);
	else
		fileSTR = generate_501();

	HGen.setStatus("501 Not Implemented");
	HGen.setType("text/html");
	HGen.setContentString(fileSTR);
	HGen.processResponse();
}

void Server::send_301(std::string location){
	HGen.clear();

	HGen.setStatus("301 Moved Permanently");
	HGen.setLocation(location);
	HGen.processResponse();
}

void Server::send_cgi(id_server_type server_id, std::string data){
	if (cgi_error(server_id)){
		return;
	}
	
	if (HGen.checkStatus())
		HGen.setStatus("200 OK");
	HGen.setContentString(data);
	
	HGen.processResponse();
}