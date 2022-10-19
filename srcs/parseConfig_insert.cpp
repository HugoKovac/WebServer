#include "../includes/parseConfig.hpp"

std::pair<std::string, std::vector<std::string> > parseConfig::insert_port( std::string raw_address ) //! Cannot pass by reference
{
	std::string address;
	std::string port;
	std::string ip_address;

	address.assign(trim_data(raw_address, "listen"));

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

	std::vector<std::string> ports(1, port);

	std::pair<std::string, std::vector<std::string> > ret(ip_address, ports);

	return ret;
}

std::vector<std::string> parseConfig::insert_server_names( std::string & raw_server_name )
{
	raw_server_name = trim_data(raw_server_name, "server_name");

	std::list<std::string> 		tmp = ft_split(raw_server_name, " ");
	std::vector<std::string> 	server_names;

	for ( std::list<std::string>::iterator it = tmp.begin() ; it != tmp.end() ; it++ )
		if (!it->empty())
			server_names.push_back(*it);

	return server_names;
}

int	parseConfig::insert_body_max_size( std::string & raw_data )
{
	std::string	data = trim_data(raw_data, "client_max_body_size");
	std::string	first_value;

	for ( std::string::size_type i = 0 ; isdigit(data[i]) ; i++ )
		first_value.append(1, data[i]);
	
	if ( first_value.empty() )
		return -1;

	return std::atoi(first_value.c_str());
}

bool parseConfig::insert_error_page( std::string & raw_error_page )
{
	std::string error_page = trim_data(raw_error_page, "error_page");
	std::list<std::string> splitted = ft_split_no_r(error_page, " \n\r\t\v\f");

	if ( splitted.size() < 2 )
	{
		// std::cout << RED << "MISSING ARGUMENT ON ERROR PAGE " << WHITE << std::endl;
		return false;
	}

	if ( isonly_digit(splitted.back()) == true )
	{
		// std::cout << RED << "LAST PARAMETER MUST BE AN ERROR PAGE PATH " << WHITE << std::endl;
		return false;
	}

	for ( std::list<std::string>::iterator it = splitted.begin() ; it != splitted.end() ; it++ )
	{
		if ( isonly_digit(*it) == false && *it != splitted.back() )
		{
			// std::cout << RED << "ERROR CODE IS ONLY DIGIT " << WHITE << std::endl;
			return false;
		}
		else if ( isonly_digit(*it) == true )
			_config.errors[std::atoi(it->c_str())] = splitted.back();
	}

	return true;
}

std::string	parseConfig::insert_root( std::string & line )
{
	std::string new_line;

	line = trim_data(line, "root");

	for ( std::string::size_type i = 0; !isspace(line[i]) && line[i] ; i++ )
		new_line.append(1, line[i]);
	
	return new_line;
}

std::pair<int, std::string> parseConfig::insert_http_redirection( std::string & raw_line )
{
	raw_line = trim_data(raw_line, "return");

	std::pair<int, std::string> ret;
	std::list<std::string>		http_redir = ft_split(raw_line, " ");

	std::list<std::string>::iterator it = http_redir.begin();
	std::list<std::string>::iterator ite = http_redir.end();

	for ( ; it != ite ; it++ )
		if (it->empty())
			it = remove_empty_line(http_redir, it);
	
	if ( http_redir.size() > 1 )
		ret = std::make_pair(std::atoi(http_redir.front().c_str()), http_redir.back());
	else
		ret = std::make_pair(-1, "");

	return ret;
}

void	parseConfig::insert_method( std::string & raw_method, const std::string & location )
{
	raw_method = trim_data(raw_method, "method_accept");

	std::list<std::string> 				methods = ft_split(raw_method, " ");
	std::list<std::string>::iterator 	it = methods.begin();
	std::list<std::string>::iterator 	ite = methods.end();

	for ( ; it != ite ; it++ )
		if (it->empty())
			it = remove_empty_line(methods, it);
	
	it = methods.begin();

	for ( ; it != ite ; it++ )
	{
		if ( *it == "GET" )
			_config.locations[location].GET = true;
		if ( *it == "POST" )
			_config.locations[location].POST = true;
		if ( *it == "DELETE" )
			_config.locations[location].DELETE = true;
	}
	
}

std::string parseConfig::insert_upload_path( std::string & line )
{
	std::string new_line;

	line = trim_data(line, "set_upload");

	for ( std::string::size_type i = 0; !isspace(line[i]) && line[i] ; i++ )
		new_line.append(1, line[i]);
	
	return new_line;
}

bool	parseConfig::insert_autoindex( std::string & raw_index )
{
	std::string index = trim_data(raw_index, "autoindex");
	std::string raw_state;
	bool 		state = false;

	for ( std::string::size_type i = 0; !isspace(index[i]) && index[i] ; i++ )
		raw_state.append(1, index[i]);
	
	if ( raw_state == "on" )
		state = true;
	else
		state = false;

	return state;
}

bool	parseConfig::insert_cgi( std::string & raw_line )
{
	std::string line = trim_data(raw_line, "cgi");

	std::list<std::string> s_line = ft_split_no_r(line, " \n\t\v\f");
	if ( s_line.size() != 2 )
	{
		parsing_error("WRONG NUMBER OF ARGUMENT IN CGI ", line);
		return false;
	}
	try
	{
		if ( s_line.front().at(0) != '.' )
		{
			parsing_error("FIRST ARGUMENT IS THE CGI EXTENSION, AND MUST START WITH A DOT: ", line);
			return false;
		}
		if ( s_line.front() != ".php" )
		{
			parsing_error("ONLY PHP IS SUPPORTED: ", line);
			return false;
		}
	}
	catch ( const std::out_of_range & e )
	{
		std::cerr << e.what() << std::endl;
		return false;
	}

	_config.cgi_path = s_line.back();
	_config.cgi_extension = s_line.front();

	return true;
}

std::string	parseConfig::insert_index( std::string & line )
{
	std::string index = trim_data(line, "index");
	std::string path;

	for ( std::string::size_type i = 0; !isspace(index[i]) && index[i] ; i++ )
		path.append(1, index[i]);
	
	return path;
}