#include "includes.hpp"

int main( int ac, char ** av )
{
	if ( ac > 2 )
	{
		// std::cout << MAGENTA << "Wrong number of argument." << std::endl;
		// std::cout << " Provide a configuration file path, or launch webserv with no argument." << WHITE << std::endl;
		return EXIT_FAILURE;
	}

	if ( ac == 2 )
	{
		parseConfig parse(av[1]);

		if ( parse.state() == false )
			return parse.exit_on_error();
	
		Server serv;
		
		serv.run( parse.get_config() );
	}
	else
	{
		parseConfig parse("conf/default.conf");

		if ( parse.state() == false )
			return parse.exit_on_error();
	
		Server serv;

		serv.run( parse.get_config() );
	}

	return EXIT_SUCCESS;
}
