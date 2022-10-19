#include "includes.hpp"


void Server::run(std::vector< struct config > confs){
	this->confs = confs;
	listenSocketServer();

	FD_ZERO(&current_connections);//init struct to 0
	FD_ZERO(&write_current_connections);//init struct to 0
	FD_ZERO(&error_current_connections);//init struct to 0
	for (std::vector<int>::size_type i = 0; i < server_sockets.size(); i++){
		FD_SET(server_sockets[i], &current_connections);//set the server socket fd in current connection struct
		FD_SET(server_sockets[i], &write_current_connections);
		FD_SET(server_sockets[i], &error_current_connections);
	}

	while (true){//server loop
		ready_connections = current_connections;
		write_ready_connections = write_current_connections;
		error_ready_connections = error_current_connections;

		/*get ready for connection fd in ready connection struct*/
		if (select(FD_SETSIZE, &ready_connections, &write_ready_connections, &error_ready_connections, NULL) == -1){
			std::cerr << "Error when select ready sockets!" << std::endl;
			exitCloseSock();
			exit (EXIT_FAILURE);
		}

		for (int i = 0; i < FD_SETSIZE; i++){//check all fd possible in ready_connection struct
			if (FD_ISSET(i, &error_ready_connections)){
				std::cerr << RED << "Kill connection (fd nb : "<< i << ")" << RED << std::endl;
				FD_CLR(i, &current_connections);
			}
			else if (FD_ISSET(i, &ready_connections)){
				if (wantToBeAccepted(i)){//new connection wait to be taken at the server.socket ip
					int clientSocket = accept_connection(i);
					FD_SET(clientSocket,  &current_connections);//set new connection established in the current_connection struct
				}
				else{
					if (!handle_connection(i, confs_index.at(i)))//keep alive
					{
						close(i);
						FD_CLR(i, &current_connections);
					}
				}
			}
		}
	}
}
