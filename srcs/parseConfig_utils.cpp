#include "../includes/parseConfig.hpp"

std::list<std::string>::iterator parseConfig::remove_empty_line( std::list<std::string> & container, std::list<std::string>::iterator & it )
{
	if (it->empty())
		container.erase(it);
	
	std::list<std::string>::iterator new_it = container.begin();

	return new_it;
}


void	parseConfig::remove_first_isspace( std::string & str )
{
	if ( str.size() >= 1)
	{
		while ( isspace(str[0]) )
			str.erase(0, 1);
	}
}

void	parseConfig::print_all_informations( void )
{
	int i = 0;
	std::cout << MAGENTA << "PRINTING ALL GATHERED INFORMATIONS " << std::endl << std::endl;
	for ( std::vector< struct config >::iterator conf_it = _configs.begin() ; conf_it != _configs.end() ; conf_it++ )
	{
		std::cout << "PRINTING CONFIG NUMBER : " << i << std::endl; 

		std::map<std::string, std::vector<std::string> >::iterator mit = conf_it->listening.begin();
		std::map<std::string, std::vector<std::string> >::iterator mite = conf_it->listening.end();

		std::vector<std::string>::iterator vit = conf_it->server_names.begin();
		std::vector<std::string>::iterator vite = conf_it->server_names.end();

		std::map< int, std::string >::iterator eit = conf_it->errors.begin();
		std::map< int, std::string >::iterator eite = conf_it->errors.end();

		std::map< std::string, struct parseLocation >::iterator lit = conf_it->locations.begin();
		std::map< std::string, struct parseLocation >::iterator lite = conf_it->locations.end();

		std::cout << "ALL LISTEN PORT AND IP " << std::endl;
		for ( ; mit != mite ; mit++ )
		{
			std::cout << mit->first << " ";
			for ( std::vector<std::string>::iterator it = mit->second.begin() ; it != mit->second.end() ; it++ )
				std::cout << *it <<" : ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << "ALL SERVER NAMES " << std::endl;
		for ( ; vit != vite ; vit++ )
			std::cout << *vit << " ";
		std::cout << std::endl;
		std::cout << "ALL ERRORS PAGE" << std::endl;
		for ( ; eit != eite ; eit++ )
		{
			std::cout << "ERROR PATH : " << eit->second << " FOR ERROR CODE -> ";
			std::cout << eit->first << std::endl;
			std::cout << std::endl;
		}
		std::cout << "CGI PARAMETER" << std::endl;
		std::cout << "CGI EXTENSION -> " << conf_it->cgi_extension << std::endl;
		std::cout << "CGI PATH -> " << conf_it->cgi_path << std::endl;
		std::cout << std::endl;

		std::cout << "ALL LOCATIONS INFORMATIONS" << std::endl;
		for ( ; lit != lite ; lit++)
		{
			std::cout << " FOR THE PATH : " << lit->first << std::endl;
			std::cout << " ROOT -> " << lit->second.root << std::endl;
			std::cout << " REPOSITORY LISTING -> ";
			if ( lit->second.autoindex == true )
				std::cout << "TRUE" << std::endl;
			else
				std::cout << "FALSE" << std::endl;
			std::cout << "HTTP REDIRECTION FOR ERROR CODE -> ";
			std::cout << lit->second.http_redirection.first << " ";
			std::cout << "WILL BE REDIRECTED TO -> " << lit->second.http_redirection.second << std::endl;
			std::cout << "EVERY ACCEPTED METHOD - > " << std::endl;
			if ( lit->second.GET == true )
				std::cout << "GET" << std::endl;
			if ( lit->second.POST == true )
				std::cout << "POST" << std::endl;
			if ( lit->second.DELETE == true )
				std::cout << "DELETE" << std::endl;
			std::cout << "UPLOADED FILES WILL BE STORED IN -> ";
			std::cout << lit->second.upload_path << std::endl;

			std::cout << std::endl;
		}
		i++;
		std::cout << std::endl;
	}
	std::cout << WHITE << std::endl;
}

int parseConfig::count_server( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
{
	int n = 0;
	int closed = 0;

	for ( ; it != ite ; it++ )
	{
		if ( it->find("server") != std::string::npos)
		{
			while ( it != ite )
			{
				if ( (it->find("}") != std::string::npos) && closed == 1 )
				{
					n++;
					break ;
				}
				if ( it->find("{") != std::string::npos )
					closed = 1;
				it++;
			}
		}
		if ( it == ite )
			break ;
	}

	return n;
}


std::string	 parseConfig::get_location_path( std::string & line )
{
	std::string new_line;

	line = trim_data(line, "location");

	for ( std::string::size_type i = 0; !isspace(line[i]) && line[i] ; i++ )
		new_line.append(1, line[i]);
	
	if ( new_line.empty() )
		return "";

	if ( ( new_line.size() >= 1 && *( new_line.end() - 1 ) != '/' ) )
	{
		// std::cout << RED << "LOCATION PATH MUST BE A FOLDER" << WHITE << std::endl;
		return "";
	}

	// for ( std::string::size_type i = 0 ; i < new_line.size() ; i++ )
	// {
	// 	if ( new_line[i] == '/' && i != new_line.size() - 1 )
	// 	{
	// 		std::cout << RED << "LOCATION PATH MUST BE ONE FOLDER " << WHITE << std::endl;
	// 		return "";
	// 	}
	// }
	
	if ( ( new_line.size() >= 2 && *( new_line.end() - 1 ) == '/' ) )
		new_line.erase( new_line.end() - 1);
	
	return new_line;
}

//! IT MUST NOT BE BY REFERENCE, or it will increment the local iterator
size_t	parseConfig::check_location( std::list<std::string>::iterator it, std::list<std::string>::iterator ite )
{
	int open = 0;
	int	closed = 0;

	for ( ; it != ite ; it++ )
	{
		if ( it->find("{") != std::string::npos )
			open++;
		
		if ( (it->find("}") != std::string::npos) )
		{
			it++;
			if ( it == ite || it->empty() || ( it->at(it->length() - 1) != ';' && it->find("location") == std::string::npos && *it != "}") )
			{
				parsing_error("MISSING A CLOSURE BRACE IN A LOCATION ");
				return std::string::npos;
			}
			closed++;
			break ;
		}
	}

	if ( open == 1 && closed == 1 )
		return 0;

	parsing_error("SOMETHING IS WRONG IN A LOCATION BRACES");

	return std::string::npos;
}

bool	parseConfig::exact_match( std::string & raw_str, const std::string & keyword )
{
	std::string 			str = trim_data(raw_str, "");
	std::string::size_type 	pos = str.find(keyword);

	if ( pos == std::string::npos )
		return false;

	if ( pos != 0 || ( !isspace(str[keyword.size()] ) ))
		return false;

	return true;
}

bool	parseConfig::check_closure( std::string & line )
{
	if ( (line.find(";") == std::string::npos) && (exact_match(line, "location") == false) && _closed == 1 && _inside == 1)
	{
		parsing_error( "MISSING ; OR UNKNOWN COMMAND : ", line);
		return false ;
	}

	if ( line.find("{") != std::string::npos )
	{
		_closed = 1;
		_inside = 1;
	}

	return true;
}

void	parseConfig::parsing_error( const std::string & error, const std::string & where )
{
	if ( !error.empty() )
		_actual_error = error;
	if ( !where.empty() )
		_actual_error.append(where);
	_state = false;
}