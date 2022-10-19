#include "includes.hpp"

std::string		Server::return_content_type( std::string URI )
{
	if (URI.size() >= 4 && URI.substr(URI.size() - 4) == ".ico" )
		return "image/ico";
	return "text/html";
}

std::string Server::autoindex( std::string URI, std::string & raw_URI )
{
	std::string listing;
	std::string empty;
	std::string subfolder;

	if (URI.empty() || *(URI.end() - 1) != '/')
		URI += "/";

	if ( URI == "/" )
		URI = "./";
	
	listing += "<html>\n<head><title>Index of /</title></head>\n<body bgcolor=\"white\">";
	listing += "\n<h1>Index of " + raw_URI + "</h1><hr><pre>\n";

	//! Now retrieving every filename of the current folder

	struct dirent *d;
	DIR *dir = opendir(URI.c_str());
	if ( dir )
		while ( (d = readdir(dir)) != NULL  )
		{
			DIR *tmp;
			if ( ( tmp = opendir((URI + d->d_name).c_str())) != NULL )
				subfolder += "/";
			listing += "<a href=\"" + empty + d->d_name + subfolder + "\">" + d->d_name + "</a>\n";
			subfolder.clear();
			closedir(tmp);
		}
	closedir(dir);
	listing += "</pre><hr></body>\n";
	listing += "</html>";
	return listing;
}	

std::string parse_uri_lite(std::string path){
	typedef std::string::size_type s_t;
	
	std::string uri = path;
	s_t pos_i;
	std::string rtn;
	pos_i = uri.find("?");
	if (pos_i == std::string::npos)
		return path;

	rtn = path.substr(0, pos_i);

	return rtn;
}

std::string Server::treat_GET_request(struct header & header, id_server_type server_id, int clientFd ) {
	std::string rtnFile;
	std::string location_name;
	std::string file = header.path;
	bool slash = false;
	bool autoindexed = false;
	header.clientFd=clientFd;

	/*have to be the first check because can change the server_id*/

	file = parse_uri(header, server_id);

	rtnFile = fileLocation(file, server_id);//routing

	location_name = retrieve_location_name(header.path, server_id);

	std::string URI = targetLocation(header.path, server_id);

	std::string responses = rtnFile + "/" + URI;
	
	while ( responses.find("//") != std::string::npos )
		responses.erase(responses.find("//"), 1);

	if ( confs[server_id].locations.find(location_name) != confs[server_id].locations.end() )
		autoindexed = confs[server_id].locations[location_name].autoindex;
	
	if ( is_folder(responses) == true )
		slash = true;
	
	if ( slash == true && autoindexed == true ) //! IF autoindexed
	{
        rtnFile = autoindex(rtnFile, header.path);
		send_200_autoindex(rtnFile);
		return "";
	}

	else if ( slash == true )
	{
		if ( confs[server_id].locations.find(location_name) != confs[server_id].locations.end() && !confs[server_id].locations[location_name].index.empty() )
		{
			// std::cout << confs[server_id].locations[location_name].index << std::endl;
			return confs[server_id].locations[location_name].index;
		}
		else
		{
			send_404(server_id);
			return "";
		}
	}

	if ( !confs[server_id].cgi_extension.empty() )
	{

		std::string tar_loc = targetLocation(header.path, server_id);
		size_t ext = tar_loc.rfind(confs[server_id].cgi_extension);

		// std::clog << "tar_loc : " << tar_loc << std::endl;
		// std::clog << "ext : " << ext << std::endl;
		// std::clog << "tar_loc[ext] : " << tar_loc[ext] << std::endl;

		if ( ext != std::string::npos && (tar_loc[ext + confs[server_id].cgi_extension.size()] == '?' || tar_loc[ext + confs[server_id].cgi_extension.size()] == '/' || !tar_loc[ext + confs[server_id].cgi_extension.size()]) ){
			std::string script_path = fileLocation(header.path, server_id) + parse_uri_lite(targetLocation(header.path, server_id));
			// std::clog << parse_uri_lite(targetLocation(header.path, server_id)) << std::endl;
			while ( script_path.find("//") != std::string::npos )
				script_path.erase(script_path.find("//"), 1);
			php_cgi(header, server_id , script_path, "GET");
			return "";
		}
	}

	std::cout << rtnFile << std::endl;

	redirect(header.path, server_id);

	// std::clog << GREEN << "[" << responses << "]" << WHITE << std::endl;
	return responses;	
}
