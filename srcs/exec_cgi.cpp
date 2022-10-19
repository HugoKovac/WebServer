#include "includes.hpp"

char **Server::create_arg(std::string php_arg){
	(void)php_arg;
	char **rtn = new char*[2];
	std::string ex = "php-cgi";


	rtn[0] = new char[ex.size()+1];
	rtn[0] = strcpy(rtn[0], (const char*)ex.c_str());
	// rtn[1] = new char[php_arg.size()+1];
	// rtn[1] = strcpy(rtn[1], (const char*)php_arg.c_str());
	rtn[1] = NULL;

	return rtn;
}

std::string Server::cgi_exec(struct header & header, id_server_type server_id, std::string php_arg, std::string method, char **env){
	FILE* fileIn = tmpfile();
	FILE* fileOut = tmpfile();
	int fdFileIn = fileno(fileIn);
	int fdFileOut = fileno(fileOut);
	int saveIn = dup(STDIN_FILENO);
	int saveOut = dup(STDOUT_FILENO);
	std::string output;

	if (method == "POST"){
		int fd_body = open("/tmp/cgi_post.log", O_RDONLY);
		char buff_body[header.content_length];
		read(fd_body, buff_body, header.content_length);
		close(fd_body);

		if (write(fdFileIn, buff_body, header.content_length) == -1){
			exitCloseSock();
			exit (1);
		}
		lseek(fdFileIn, 0, SEEK_SET);
		// char test[header.content_length];
		// read(fdFileIn, test, header.content_length);
		// std::clog << "["<< test << "]"<< std::endl;
	}

	int pid = fork();

	if (pid < 0){
		std::cerr << "Error when forking for php-cgi" << std::endl;
		return ("Status: 500\n\n");
	}

	if (pid == 0){
		char ** arg = create_arg(php_arg);

		dup2(fdFileIn, STDIN_FILENO);
		close(fdFileIn);
		dup2(fdFileOut, STDOUT_FILENO);
		close(fdFileOut);

		execve(confs[server_id].cgi_path.c_str(), arg, env);

		for (size_t i = 0; arg[i]; i++){
			// std::clog << "=" << arg[i] << "=" << std::endl;
			delete[] arg[i];
		}
		delete[] arg;
		if (write(STDOUT_FILENO, "Status: 500\n\n", 13) == -1){
			exitCloseSock();
			exit (1);
		}
		exit (1);
	}
	else{
		wait(NULL);
		lseek(fdFileOut, 0, SEEK_SET);
		
		ssize_t rtn = 1;
		char buff[100];

		while (rtn){
			memset(buff, 0, 100);
			rtn = read(fdFileOut, buff, 99);
			// std::clog << "-> " << buff << std::endl;
			output.append(buff);
		}
	}
	
	dup2(saveIn, STDIN_FILENO);
	dup2(saveOut, STDOUT_FILENO);
	fclose(fileIn);
	fclose(fileOut);
	close(saveIn);
	close(saveOut);

	// std::clog << "\""<< output<< "\"" << std::endl;
	return output;
}
