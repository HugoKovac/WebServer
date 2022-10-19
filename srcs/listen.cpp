#include "includes.hpp"

#define BACKLOG 10

void Server::listenSocketServer(){
	std::vector<std::string> ald_port;
	for (std::vector< struct config >::size_type k = 0; k < confs.size(); k++){//for all servers
		std::map< std::string, std::vector<std::string> >::iterator it = confs[k].listening.begin();
		while (it != confs[k].listening.end()){//for all addresses
			for (std::map< std::string, std::vector<std::string> >::size_type i = 0; i < it->second.size(); i++){//for all ports
				if (std::find(ald_port.begin(), ald_port.end(), it->second[i]) == ald_port.end()){
					ald_port.push_back(it->second[i]);
					sockaddr_in	serverSocketStruct;

					server_sockets.push_back( socket(AF_INET, SOCK_STREAM, 0) );

					if (server_sockets.at(server_sockets.size() - 1) == -1){
						for (std::string::size_type j = 0; j < server_sockets.size() && server_sockets[j] != -1; j++)
							close(server_sockets[j]);
						std::cerr << "Error when creating server's socket!" << std::endl;
						exit (EXIT_FAILURE);
					}

					int value = 1;
					if (setsockopt(server_sockets.at(server_sockets.size() - 1), SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0)
						exit (EXIT_FAILURE);

					serverSocketStruct.sin_family = AF_INET;
					serverSocketStruct.sin_addr.s_addr = INADDR_ANY;
					serverSocketStruct.sin_port = htons(atoi(it->second[i].c_str()));

					server_sockets_struct.push_back( serverSocketStruct );
					socket_to_server.push_back(k);


					// std::cout << it->second[i] << std::endl;

					if (bind(server_sockets.at(server_sockets.size() - 1),
						reinterpret_cast<struct sockaddr *>(&serverSocketStruct),
						sizeof(serverSocketStruct)) == -1){
							std::cerr << "Error when binding socket and address!" << std::endl;
							for (std::vector<int>::size_type j = 0; j < server_sockets.size() && server_sockets[j] != -1; j++)
								close(server_sockets[j]);
							exit (EXIT_FAILURE);
					}

					if (listen(server_sockets.at(server_sockets.size() - 1), BACKLOG) == -1){
							std::cerr << "Error when server started listening!" << std::endl;
							exitCloseSock();
							exit (EXIT_FAILURE);
					}
				}
			}
			++it;
		}
	}
}
