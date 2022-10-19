#include "../includes/includes.hpp"

bool		Server::check_GET_request_validity( struct header & header, id_server_type &server_id )
{
	std::string location_name = retrieve_location_name(header.path, server_id);
	
	if ( confs[server_id].locations.find(location_name) != confs[server_id].locations.end() )
	{
		if ( confs[server_id].locations[location_name].GET == false )
		{
			send_405(server_id);
			return false;
		}
	}
	return true;
}


bool		Server::check_DELETE_request_validity( struct header & header, id_server_type &server_id )
{
	std::string location_name = retrieve_location_name(header.path, server_id);
		
	if ( confs[server_id].locations.find(location_name) != confs[server_id].locations.end() )
	{
		if ( confs[server_id].locations[location_name].DELETE == false )
		{
			send_405(server_id);
			return false;
		}
	}
	return true;
}

bool		Server::check_POST_request_validity( struct header & header, id_server_type & server_id )
{
	std::string location_name = retrieve_location_name(header.path, server_id);

	if ( confs[server_id].locations.find(location_name) != confs[server_id].locations.end() )
	{
		if ( header.content_length > confs[server_id].locations[location_name].body_max_size && header.method == POST )
		{
			send_413(server_id);
			return false;
		}
		if ( confs[server_id].locations[location_name].POST == false )
		{
			send_405(server_id);
			return false;
		}
	}
	return true;
}

void Server::get_request( Request & client_request, int clientSocket, id_server_type &server_id )
{
	std::string to_send;
	try
	{
		to_send = treat_GET_request(client_request.get_header(), server_id, clientSocket);
		if (!to_send.empty()){
			send_responses(to_send);
		}
	}
	catch ( const Error_page & page )
	{
		std::string err = page.what();
		if (err == "400")
			send_400(server_id);
		else if (err == "404")
			send_404(server_id);
		else
		{
			check_server_name(client_request.get_header(), server_id);
			send_301(confs[server_id].locations[err].http_redirection.second);
		}
	}
}

void Server::delete_request( Request & client_request, id_server_type &server_id )
{
	treat_DELETE_request(client_request.get_header(), server_id);
}

void Server::post_request( Request & client_request, id_server_type &server_id )
{
	if ( treat_POST_request(client_request.get_header(), client_request.get_body(), client_request.get_file_path(), server_id) == true )
		send_201();
}

bool Server::reject_client( int clientSocket, std::string & file_to_delete )
{
	if ( send_client_response(clientSocket) < 0 ) //? Send final response
	{
		std::cerr << RED << " FATAL ERROR SENDING RESPONSE " << WHITE << std::endl;
		exit(1);
	}

	all_request.erase(_old_id);
	remove(file_to_delete.c_str());
	return false;
}

bool Server::treat_request( Request & client_request, int clientSocket, id_server_type & server_id )
{
	client_request.read_client(clientSocket);

	_old_id = check_server_name(client_request.get_header(), server_id);

	if ( client_request.get_actual_error() == 414 )
	{
		send_414(server_id);
		return reject_client(clientSocket, client_request.get_file_path());
	}

	if ( client_request.get_actual_error() == 400 )
	{
		send_400(server_id);
		return reject_client(clientSocket, client_request.get_file_path());
	}

	if ( client_request.get_header().path.size() > 1000 )
	{
		send_414(server_id);
		return reject_client(clientSocket, client_request.get_file_path());
	}

	if ( client_request.get_header().method == GET )
	{
		if ( check_GET_request_validity(client_request.get_header(), server_id ) == false )
			return reject_client(clientSocket, client_request.get_file_path());
		else
			get_request(client_request, clientSocket, server_id);
	}

	else if ( client_request.get_header().method == POST )
	{
		if ( check_POST_request_validity(client_request.get_header(), server_id ) == false )
			return reject_client(clientSocket, client_request.get_file_path());
		else if ( client_request.is_valid_request() == false )
		{
			send_204();
			return reject_client(clientSocket, client_request.get_file_path());
		}
		else if ( client_request.is_full() == false )
			return true;
		else
			post_request( client_request, server_id );
	}
	else if ( client_request.get_header().method == DELETE )
	{
		if ( check_DELETE_request_validity(client_request.get_header(), server_id ) == false )
			return reject_client(clientSocket, client_request.get_file_path());
		else
			delete_request( client_request, server_id );
	}
	else
		send_501(server_id);
	
	if ( send_client_response(clientSocket) < 0 ) //? Send final response
		exit(1);
	
	remove(client_request.get_file_path().c_str());
	all_request.erase(_old_id);
	return client_request.get_header().keep_alive;
}

int	Server::send_client_response( int clientSocket )
{
	// std::clog << "\"" << HGen.getStr() << "\"" << std::endl;
	if ( send(clientSocket, HGen.getStr().c_str(), HGen.getStr().size(), SOCK_DGRAM) < 0 )
	{
		std::cerr << MAGENTA << " FATAL ERROR SENDING CLIENT RESPONSE. ABORTING " << WHITE << std::endl;
		return -1;
	}
	return 0;
}

bool Server::handle_connection( int clientSocket, id_server_type server_id )
{
	if ( all_request.find(server_id) == all_request.end() )
	{
		Request request;
		all_request.insert(std::make_pair(server_id, request));
	}

	return treat_request(all_request[server_id], clientSocket, server_id );
}
