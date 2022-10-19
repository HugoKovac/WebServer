#include "../includes/server.hpp"

bool	Server::treat_POST_request( struct header & head, struct body & bod, const std::string & file, id_server_type server_id )
{
	std::ifstream 	tmp(file.c_str(), std::ifstream::binary ); //? We first open the raw_data file
	std::fstream 	new_file;

	std::string location_name = retrieve_location_name(head.path, server_id);
	
	std::string		path = confs[server_id].locations[location_name].upload_path + "/";
	while ( path.find("//") != std::string::npos )
		path.erase(path.find("//"), 1);

	if ( head.path.size() > 4 && head.path.substr(head.path.size() - 4) == ".php" )
	{
		// std::clog << "1\n";
		if ( head.content_type == "multipart/form-data" )
		{
		// std::clog << "2\n";

			std::string filename;
			std::string line;
			std::string content;

			while ( content.find("\r\n\r\n") == std::string::npos ) //? We skip the first header part, because we just want the body
			{
				std::getline(tmp, line);
				content += line;
				content += '\n';
			}

			while ( std::getline(tmp, line) ) //? We iterate trough the file with a gnl until we find the last boundary 
			{
				std::string file_path = "/tmp/cgi_post.log";
				bod.body_path = file_path;
				new_file.open(file_path.c_str(), std::ios::out);
				
				if ( !new_file.is_open() )
				{
					send_500(server_id); //! for now its an internal server error 500
					tmp.close();
					remove(file.c_str());
					return false;
				}
				std::string tmp_line;
				while ( std::getline(tmp, tmp_line) )
				{
					new_file << tmp_line;
					new_file << '\n';
				}
				new_file.close();

				std::string script_path = fileLocation(head.path, server_id) + targetLocation(head.path, server_id);
				while ( script_path.find("//") != std::string::npos )
					script_path.erase(script_path.find("//"), 1);
				php_cgi(head, server_id , script_path, "POST");
				remove("/tmp/cgi_post.log");
				tmp_line.clear();
				tmp.close();
				remove(file.c_str());
				return false;
			}
		}
		else
		{ //! Need to find how to store it
		// std::clog << "3\n";
		
			std::string filename;
			std::string line;
			std::string content;

			while ( content.find("\r\n\r\n") == std::string::npos ) //? We skip the first header part, because we just want the body
			{
				std::getline(tmp, line);
				content += line;
				content += '\n';
			}

			std::string file_path = "/tmp/cgi_post.log";
			bod.body_path = file_path;
			new_file.open(file_path.c_str(), std::ios::out);
			if ( !new_file.is_open() )
			{
				send_500(server_id);
				tmp.close();
				remove(file.c_str());
				return false;
			}

			while ( std::getline(tmp, line) )
				new_file << line;
			
			new_file.close();

			std::string script_path = fileLocation(head.path, server_id) + targetLocation(head.path, server_id);
			while ( script_path.find("//") != std::string::npos )
				script_path.erase(script_path.find("//"), 1);

			// std::clog << server_id << std::endl;

			php_cgi(head, server_id , script_path, "POST");
			remove("/tmp/cgi_post.log");
			tmp.close();
			remove(file.c_str());
			return false;
		}
	}
	else if ( head.content_type == "multipart/form-data" )
	{
		std::string filename;
		std::string line;
		std::string content;

		while ( content.find("\r\n\r\n") == std::string::npos ) //? We skip the first header part, because we just want the body
		{
			std::getline(tmp, line);
			content += line;
			content += '\n';
		}

		while ( std::getline(tmp, line) || line == head.boundary + "--\r" ) //? We iterate trough the file with a gnl until we find the last boundary 
		{
			if ( line.substr(0, 20) == "Content-Disposition:" )
			{
				if ( line.find("filename") == std::string::npos )
				{
					std::cerr << "No file name found in the body! Make a proper form" << std::endl;
					send_400(server_id);
					tmp.close();
					remove(file.c_str());
					return false;
				}
				filename = line.substr(line.find("filename") + 10, filename.find("\""));
				filename = filename.substr(0, filename.find("\""));
				std::cout << RED << "[" << filename << "]" << WHITE << std::endl;
			}
			else if ( line.substr(0, 13) == "Content-Type:" )
				continue ;
			else if ( line == "\r" ) //? between boundary you first find informations like content disposition or content type, those informations are separated by a carriage return from the binary file
			{
				std::string file_path = path + filename;
				bod.body_path = file_path;

				std::cout << "OPENING " << file_path << std::endl;
				if ( file_already_exist(file_path) == true )
				{
					std::cout << "FILE ALREADY EXIST" << std::endl;
					send_202();
					tmp.close();
					remove(file.c_str());
					return false;
				}

				new_file.open(file_path.c_str(), std::ios::out);
				if ( !new_file.is_open() )
				{
					send_500(server_id);
					tmp.close();
					remove(file.c_str());
					return false;
				}
				std::string tmp_line;
				std::string offset;
				std::getline(tmp, tmp_line); 
				while ( std::getline(tmp, offset) )
				{
					if ( offset == head.boundary + "\r" || offset == head.boundary + "--\r" )
					{
						if ( tmp_line.size() >= 1 )
							tmp_line.erase(tmp_line.end() - 1);
						new_file << tmp_line;
						break ;
					}

					new_file << tmp_line;
					new_file << '\n';
					tmp_line = offset;
				}
				new_file.close();
				tmp_line.clear();
			}
			if ( line == head.boundary + "--\r" || line.empty() )
				break ;
		}
	}
	else
	{ //! Need to find how to store it
		// std::clog << "5\n";
		std::string filename;
		std::string line;
		std::string content;

		while ( content.find("\r\n\r\n") == std::string::npos ) //? We skip the first header part, because we just want the body
		{
			std::getline(tmp, line);
			content += line;
			content += '\n';
		}

		std::string file_path = "/tmp/default.txt";
		bod.body_path = file_path;

		if ( file_already_exist(file_path) == true )
		{
			send_202();
			tmp.close();
			remove(file.c_str());
			return false;
		}

		new_file.open(file_path.c_str(), std::ios::out);
		if ( !new_file.is_open() )
		{
			send_500(server_id);
			tmp.close();
			remove(file.c_str());
			return false;
		}

		while ( std::getline(tmp, line) )
			new_file << line;
	
		new_file.close();
	}
	tmp.close();
	remove(file.c_str());
	return true;
}