#include "includes.hpp"

int Server::accept_connection(int fdServer){
	int clientFd;
	int serverIndex = findServerIndex(fdServer);
	socklen_t serverInfoSize = static_cast<socklen_t>(sizeof(server_sockets[serverIndex]));

	clientFd = accept(server_sockets[serverIndex], reinterpret_cast<struct sockaddr *>(&server_sockets_struct[serverIndex])
		, &serverInfoSize);
	confs_index[clientFd] = socket_to_server[serverIndex];
	if (clientFd == -1){
		std::cerr << "Error when accept new connection!" << std::endl;
		exitCloseSock();
		exit (EXIT_FAILURE);
	}

	return clientFd;
}
