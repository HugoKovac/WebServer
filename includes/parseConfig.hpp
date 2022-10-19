#ifndef PARSECONFIG_HPP
# define PARSECONFIG_HPP

#include "includes.hpp"

struct parseLocation 
{
	long long         					body_max_size;
	int	 	 							location_type;
	bool 								GET;
	bool 								POST;
	bool 								DELETE;
	bool 								autoindex;
	std::string							index;
	std::string 						root;
	std::string 						upload_path;
	std::pair< int, std::string >		http_redirection;

	parseLocation( void ){ body_max_size = 100000; autoindex = false; GET = false; POST = false; DELETE = false; upload_path = "./"; }
	~parseLocation ( void ) {}
};

struct config 
{
	std::string 															query_string;
	std::string																cgi_extension;
	std::string																cgi_path;
	std::vector<std::string> 												server_names;
	std::map<std::string/*ip*/, std::vector<std::string> /*ports list*/> 	listening;
	std::map< int, std::string > 											errors;
	std::map< std::string, struct parseLocation, greater<std::string> > 	locations;

	config( void ) { locations["/"].index = "default/index.html"; }
	~config ( void ) {}

	void clear( void )
	{
		locations.clear();
		server_names.clear();
		listening.clear();
		errors.clear();	
		cgi_extension.clear();
		cgi_path.clear();
		query_string.clear();
		
		locations["/"].index = "default/index.html";
	}
};


class parseConfig
{
	public:


		parseConfig( std::string path = std::string("conf/default.conf") );
		~parseConfig( void );


		int													exit_on_error( void );

		//? Getter
		bool 												state( void );
		size_t												get_server_number( void );
		std::vector< struct config > 						get_config( void );




	private:

		bool									_found;
		size_t									_closed;
		bool									_inside;
		std::list<std::string> 					_file;
		std::string		 						_file_path;


		struct parseLocation 					_parseLocation;
		struct config 							_config;
		std::vector< struct config > 			_configs;


		std::string 							_actual_error;
		bool 									_state;


		//? Main
		bool    											fill_file( void );
		void												parse_file( void );
		void												set_default_value( void );
		bool			 					 				check_conf_name( std::string & str );
		bool												check_fd_system( void );

		//? Insertion
		std::string 										insert_upload_path( std::string & line );
		std::pair<std::string, std::vector<std::string> >	insert_port( std::string raw_address );
		std::pair<int, std::string >						insert_http_redirection( std::string & raw_line );
		bool												insert_error_page( std::string & raw_error_page );
		std::vector<std::string>							insert_server_names( std::string & raw_server_name );
		bool												insert_autoindex( std::string & raw_index );
		void												insert_method( std::string & raw_method, const std::string & location );
		int													insert_body_max_size( std::string & raw_data );
		std::string		 	 		 	 					insert_root( std::string & line );
		bool												insert_cgi( std::string & raw_line );
		std::string											insert_index( std::string & line );


		//? Utils
		std::list<std::string>::iterator		 			parse_location( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );
		std::string	 	 		 	 						get_location_path( std::string & line );
		size_t												check_location( std::list<std::string>::iterator it, std::list<std::string>::iterator ite );
		int													count_server (std::list<std::string>::iterator it , std::list<std::string>::iterator ite );
		bool												search_informations( std::string & line );
		bool												check_closure( std::string & line );
		std::list<std::string>::iterator 					remove_empty_line( std::list<std::string> & container, std::list<std::string>::iterator & it );
		bool			 								 	exact_match( std::string & raw_str, const std::string & keyword );
		void    											remove_first_isspace( std::string & str );
		void												parsing_error( const std::string & error, const std::string & where = std::string() );
		void    											print_all_informations( void );	


		//? Common
		std::list<std::string>								ft_split( const std::string & header, const std::string & charset );	

		//? Default Constructor
		parseConfig( void );
};

#endif