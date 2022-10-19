#ifndef SERVER_HPP
# define SERVER_HPP

# define D_200 0
# define D_400 1

# include "includes.hpp"

class Server{
	//exceptions
	class Error_page : public std::exception{
		public:
		explicit Error_page(const char* error_msg) : msg(error_msg) {}
		explicit Error_page(const std::string& error_msg) : msg(error_msg) {}
		virtual ~Error_page() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {}

		virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW{
			return msg.c_str();
		}

		protected:
		std::string msg;
	};


	typedef std::vector< struct config >::size_type id_server_type;

	//var
	id_server_type _old_id;
	std::string _header;std::map<std::string/*port*/, std::vector<std::string>/*server_names*/> mutiple_server_port;
	std::vector< struct config > confs;
	std::vector<int> server_sockets;
	std::vector< std::vector< struct config >::size_type > socket_to_server;
	std::map<int/*client fd*/, int/*server_id*/> confs_index;
	std::vector<sockaddr_in> server_sockets_struct;
	fd_set current_connections;//fd waiting to communicate
	fd_set ready_connections;//fd ready to communicate
	fd_set write_current_connections;
	fd_set write_ready_connections;//! en faire un tableau
	fd_set error_current_connections;
	fd_set error_ready_connections;
	HeaderGen HGen;
	bool cgi_on;
	std::map<id_server_type/*socket id*/, class Request /*Request Object*/> all_request;


	//func

	int		send_client_response( int clientSocket );

	bool 	handle_connection(int clientSocket, id_server_type server_id);
	bool 	treat_request( Request & client_request, int clientSocket, id_server_type & server_id );


	//? Error generator
	
	std::string generate_400( void );
	std::string generate_404( void );
	std::string generate_405( void );
	std::string generate_413( void );
	std::string generate_414( void );
	std::string generate_500( void );
	std::string generate_501( void );

	//? Error generator

	std::string get_error_page( std::string fileName );
	int accept_connection(int fdServer);
	void receive_request_body( struct request & req, int requestFd );
	std::string fileLocation(std::string request, id_server_type serverNb);
	std::string	targetLocation( std::string URI, id_server_type server_id );
	std::string retrieve_location_name( std::string URI, id_server_type server_id );
	std::string fileToString(std::string fileName, bool error = false);
	void listenSocketServer();
	std::string ipToHost(std::string hostname);
	std::vector< struct config >::size_type check_server_name(struct header & header, id_server_type &id);
	std::string parse_uri(struct header & header, id_server_type server_id);
	void redirect(std::string file, id_server_type server_id);
	void send_responses(std::string file);
	void send_200_autoindex(std::string content);
	void send_200(std::string file, std::string & extension );
	void send_200_chunked( void );
	void send_201( void );
	void send_204( void );
	void send_202( void );
	void send_400(id_server_type serverNb);
	void send_413(id_server_type serverNb);
	void send_414( id_server_type serverNb );
	void send_404(id_server_type serverNb);
	void send_405(id_server_type serverNb);
	void send_301(std::string location);
	void send_500(id_server_type serverNb);
	void send_501(id_server_type serverNb);
	void send_cgi(id_server_type server_id, std::string data);
	bool cgi_error(id_server_type server_id);
	std::string parseCgiHeader(std::string buffer);
	bool isIpAddress(std::string addr);
	bool hostToIp(std::string hostname);
	int findServerIndex(int fdServer);
	void exitCloseSock();
	bool wantToBeAccepted(int fd);
	char **cgi_vars(struct header & header, id_server_type server_id, std::string php_arg, std::string method);
	void php_cgi(struct header & header, id_server_type server_id, std::string script_name, std::string method);
	std::string cgi_exec(struct header & header, id_server_type server_id, std::string php_arg, std::string method, char **env);
	char **create_arg(std::string php_arg);


	bool reject_client( int clientSocket, std::string & file_to_delete );

	//* DELETE
	
	void	delete_request( Request & client_request, id_server_type &server_id );
	void	treat_DELETE_request( struct header & head, id_server_type &server_id );
	bool	check_DELETE_request_validity( struct header & header, id_server_type &server_id );

	//* GET
	void get_request( Request & client_request, int clientSocket, id_server_type &server_id );
	bool	check_GET_request_validity( struct header & header, id_server_type &server_id );
	std::string autoindex( std::string URI, std::string & raw_URI  );
	std::string treat_GET_request(struct header & header, id_server_type serverNb, int clientFd);
	std::string return_content_type( std::string URI );


	//* POST
	std::string get_file_name( const std::string & line );
	void		post_request( Request & client_request, id_server_type &server_id );
	bool 		treat_POST_request( struct header & head, struct body & bod, const std::string & file, id_server_type server_id );
	bool		check_POST_request_validity( struct header & header, id_server_type & server_id );



public:
	Server() : cgi_on(false){}
	void run(std::vector< struct config > confs);

};

#endif