/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarouf <cmarouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 13:50:09 by cmarouf           #+#    #+#             */
/*   Updated: 2022/10/18 15:46:12 by cmarouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

Request::Request( void ) : _read_content_length(0), _error_code(0), _is_valid(false), _is_full(false), _is_chunked(false), _with_body(false), _header_found(false)
{
	search_tmp_name();
}

Request::~Request( void )
{
	
}

Request::Request( Request const & src )
{
	*this = src;
}


Request & Request::operator=( Request const & src )
{
	_tmp_filename = src._tmp_filename;
	_read_content_length = src._read_content_length;	
	_is_full = src._is_full;
	_with_body = src._with_body;

	_header.host = src._header.host;
	_header.port_host = src._header.port_host;
	_header.path = src._header.path;
	_header.header_str = src._header.header_str;
	_header.boundary = src._header.boundary;
	_header.content_type = src._header.content_type;
	_header.connection = src._header.connection;
	_header.method = src._header.method;
	_header.content_length = src._header.content_length;
	_header.keep_alive = src._header.keep_alive;
	_body.body_path = src._body.body_path;
	_body.type = src._body.type;
	_body.length = src._body.length;

	return *this;
}

void	Request::search_tmp_name( void )
{
	std::stringstream 	out;
	std::string 		name = TMP_FILE_NAME;
	std::string 		s_id;
	size_t 				id = 174;
	
	while ( 1 )
	{
		out << id;
		s_id = out.str();
		
		if ( file_already_exist(name + s_id) == false )
		{
			_tmp_filename = name + s_id;
			return ;
		}
		
		s_id.clear();
		out.str("");
		id++;
	}
}

void Request::read_client( int requestFd )
{
    char 			buffer[8192 + 1];
	std::fstream	file( _tmp_filename.c_str(), std::fstream::app | std::fstream::binary );
	size_t 			end;

	memset( buffer, 0, 8192 );
	if ( ( end = recv(requestFd, buffer, 8192 - 1, 0)) > 0 )
		insert(buffer, end, file);
	if ( check_if_header_is_received() == true )
		read_header();
	file.close();
}

void	Request::insert( char * buffer, size_t len, std::fstream & file )
{
	file.write(buffer, len);
	
	_read_content_length += len;

	if ( _with_body == true )
		if ( _read_content_length >= _header.content_length )
			_is_full = true;
	file.close();
}

bool	Request::check_if_header_is_received( void )
{
	_error_code = 0;
	std::string	 	buffer;
	std::string		full_buffer;
	std::ifstream 	tmp(_tmp_filename.c_str(), std::ifstream::binary );
	
	while ( std::getline(tmp, buffer) )
	{
		full_buffer += buffer;
		full_buffer += '\n';
		
		if ( full_buffer.size() > 2000 )
		{
			_error_code = 414;
			return false;
		}

		if ( full_buffer.rfind("\r\n\r\n") != std::string::npos )
		{
			std::clog << YELLOW << full_buffer << WHITE << std::endl;
			_header_buffer = full_buffer;
			return true;
		}
	}
	return false;
}

void	Request::read_header( void )
{
	_error_code = 0;
	std::istringstream 	tmp(_header_buffer);
	std::string 		buff;
	bool 				first = false;
	bool	 			is_there_host = false;

	while ( std::getline(tmp, buff) )
	{
		std::list<std::string> infos = ft_split_no_r(buff, " \r");

		if ( infos.size() == 3 && (infos.front() == "GET" || infos.front() == "POST" || infos.front() == "DELETE") )
		{
			_header.method = assign_method(infos.front());
			if ( infos.back() != "HTTP/1.1" )
			{
				_error_code = 400;
				return ;
			}
			infos.pop_back();
			_header.path = infos.back();
			first = true;
		}
		else if ( first == false )
		{
			_header.method = UNKNOWN;
			return ;
		}

		else if ( infos.size() == 2 && infos.front() == "Host:" )
		{
			assign_host(infos.back());
			is_there_host = true;
		}
		else if ( infos.size() == 1 && infos.front().find("Host:") != std::string::npos )
		{
			std::string host = trim_data(infos.front(), "Host:");
			assign_host(host);
			is_there_host = true;
		}

		else if ( infos.size() == 2 && infos.front() == "Content-Length:" ) //! If content_length is missing and the method is POST, must throw an 411
		{
			_with_body = true;
			_header.content_length = -1;
			_header.content_length = std::atoi(infos.back().c_str());
			if ( _header.content_length <= 0 )
			{
				_error_code = 400;
				return ;
			}
			if ( _read_content_length >= _header.content_length ) //? Here we check if we didnt get the full body while reading the header
				_is_full = true;
			_is_valid = true;
		}

		else if ( infos.size() == 2 && infos.front() == "Transfer-Encoding:" )
		{
			if ( infos.back() == "chunked" )
				_is_chunked = true;
		}
		
		else if ( infos.size() == 2 && infos.front() == "Connection:" )
		{
			if ( infos.back() == "keep_alive")
				_header.keep_alive = true;
			else
				_header.keep_alive = false;
		}

		else if ( infos.size() == 3 && infos.front() == "Content-Type:" )
		{
			infos.pop_front();
			_header.content_type = infos.front();
			_header.raw_content_type = infos.front() + " " + infos.back();
			_header.boundary = "--" + infos.back().substr(9);		
			_header.content_type.erase(_header.content_type.size() - 1);
			_is_valid = true;
		}

		else if ( infos.size() == 2 && infos.front() == "Content-Type:" )
		{
			infos.pop_front();
			_header.content_type = infos.front();
			_header.raw_content_type = _header.content_type;
			_is_valid = true;
		}
		
	}
	if ( is_there_host == false )	
	{
		_error_code = 400;
		return ;
	}
}

struct header & Request::get_header( void )
{
	return _header;
}

bool	Request::is_valid_request( void )
{
	return _is_valid;
}

struct body & Request::get_body( void )
{
	return _body;
}

int					Request::get_actual_error( void )
{
	return _error_code;
}

std::string &		Request::get_curr_response( void )
{
	return _curr_response;
}

bool		 		Request::is_full( void )
{
	return _is_full;
}

std::string &	 	Request::get_file_path( void )
{
	return _tmp_filename;
}

int	Request::assign_method( const std::string & method_name )
{
	if ( method_name == "GET" )
		return GET;
	else if ( method_name == "POST" )
		return POST;
	else if ( method_name == "DELETE" )
		return DELETE;
	else
		return UNKNOWN;
}

void	Request::assign_host( std::string & line )
{
	std::string address;
	std::string port;
	std::string ip_address;

	address.assign(trim_data(line, "Host:"));
	if ( address.find(":") != std::string::npos )
	{
		ip_address = address.substr(0, address.find(":"));
		port = address.substr(address.find(":") + 1, std::string::npos );
	}
	else if ( address.find(".") != std::string::npos )
	{
		ip_address.assign(address);
		port.assign("8080");
	}
	else if ( address == "localhost" )
	{
		port.assign("8080");
		ip_address.assign("localhost");
	}
	else
	{
		port.assign(address);
		ip_address.assign("localhost");
	}

	_header.port_host = port;
	_header.host = ip_address;
}
