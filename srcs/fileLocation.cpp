#include "includes.hpp"

std::string	 	 Server::retrieve_location_name( std::string URI, id_server_type server_id )
{
	if ( URI == "/" )
		return URI;
	
	if ( URI.size() >= 2 && URI[0] == '/' )
		URI.erase(0, 1);

	std::map< std::string, struct parseLocation >::iterator it = confs[server_id].locations.begin();
	std::map< std::string, struct parseLocation >::iterator ite = confs[server_id].locations.end();

	for ( ; it != ite ; it++ )
	{
		if ( it->first != "/" && URI.compare(0, it->first.size() + 1, it->first + "/") == 0 )
			return it->first;
		else if ( it->first != "/" &&  it->first == URI )
			return it->first;
	}

	return "/";
}

std::string	 	 Server::targetLocation( std::string URI, id_server_type server_id )
{
	if ( URI == "/" )
		return URI;
	
	if ( URI.size() >= 2 && URI[0] == '/' )
		URI.erase(0, 1);
	
	std::map< std::string, struct parseLocation >::iterator it = confs[server_id].locations.begin();
	std::map< std::string, struct parseLocation >::iterator ite = confs[server_id].locations.end();

	for ( ; it != ite ; it++ )
	{
		if ( it->first != "/" && URI.compare(0, it->first.size(), it->first) == 0 )
		{
			URI.erase(0, it->first.size());
			break ;
		}
	}

	if ( it == ite )
		if ( URI.compare( 0, 1, "/" ) == 0 )
			URI.erase(0, 1);


	return URI;
}

std::string Server::fileLocation(std::string request, std::vector< struct config >::size_type server_id){
	std::string::size_type slash;//postion de slash (dir/other_dir) entre dir et other_dir
	std::string location;
	std::string rtn;

	if (request.rfind("/") == request.size() - 1)//if / at the end
		request = request.substr(0, request.size() - 1);

	if (request.find("/") == 0)//if / at the begining
		request = request.substr(1, request.size() - 1);

	//find 1st location
	slash = request.find("/");
	if (slash == 0 && request.size() > 1){
		request = request.substr(slash+1, request.size());
		slash = request.find("/");
	}

	if (slash != std::string::npos && request[slash + 1])
		location = request.substr(0, slash);
	else
		location = request;

	// std::cout << GREEN << "[" << location << "]" << WHITE << std::endl;
	//check if config exist
	if (location.size() && confs[server_id].locations.find(location) != confs[server_id].locations.end() && !confs[server_id].locations.find(location)->second.root.empty()){//cas de dir/file.html
		if (confs[server_id].locations[location].root[confs[server_id].locations[location].root.size() - 1] != '/')//si pas de / a la fin du root ajouter
			confs[server_id].locations[location].root += "/";
		rtn.append(confs[server_id].locations[location].root);//ajouter le root de la config
		// if (slash != std::string::npos)
		// 	rtn.append(request.substr(slash + 1, request.size()));//ajouter ce qu'il y a apres le /
	}
	else if (location.size() && confs[server_id].locations.find("/") != confs[server_id].locations.end() && !confs[server_id].locations.find("/")->second.root.empty()){//check if / config exist
		rtn.append(confs[server_id].locations["/"].root);
		if (confs[server_id].locations["/"].root[confs[server_id].locations["/"].root.size() - 1] != '/')
			confs[server_id].locations["/"].root += "/";
		// if (slash != std::string::npos)
		// 	rtn.append(request.substr(slash + 1, request.size()));
	}
	else if (confs[server_id].locations.find(request) != confs[server_id].locations.end() && !confs[server_id].locations.find(request)->second.root.empty()){//only file.html with config
		if (confs[server_id].locations[request].root[confs[server_id].locations[request].root.size() - 1] != '/')
			confs[server_id].locations[request].root += "/";
		rtn.append(confs[server_id].locations[request].root);
		rtn.append(request.substr(0, request.size()));
	}
	else if (confs[server_id].locations.find("/") != confs[server_id].locations.end() && !confs[server_id].locations.find("/")->second.root.empty()){//only file.html with "/" config
		if (confs[server_id].locations["/"].root[confs[server_id].locations["/"].root.size() - 1] != '/')
			confs[server_id].locations["/"].root += "/";
		rtn.append(confs[server_id].locations["/"].root);
		rtn.append(request.substr(0, request.size()));
	}
	else
		rtn.append(request);

	// std::clog << "Return : " << rtn << std::endl;
	// std::clog << "Location : " << location << std::endl;
	// std::clog << "Request : " << request << std::endl;
	// std::cout << MAGENTA << "[" << rtn << "]" << WHITE << std::endl;
	return rtn;
}
/*
	return le path sans / au debut et a la fin
	si / return empty string
*/