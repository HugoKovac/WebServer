#ifndef INCLUDES_HPP
# define INCLUDES_HPP

# define BLACK   "\033[1m\033[30m"
# define RED     "\033[1m\033[31m"
# define GREEN   "\033[1m\033[32m"
# define YELLOW  "\033[1m\033[33m"
# define BLUE    "\033[1m\033[34m"
# define MAGENTA "\033[1m\033[35m"
# define CYAN    "\033[1m\033[36m"
# define WHITE   "\033[1m\033[37m"
# define TMP_FILE_NAME "/tmp/tmp_filex"

# define SSTR( x ) static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x ) ).str()

# include <vector>
# include <list>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <iostream>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <sys/select.h>
# include <sys/time.h>
# include <sys/types.h>
# include <dirent.h>
# include <unistd.h>
# include <string>
# include <fstream>
# include <istream>
# include <sys/stat.h>
# include <cstring>
# include <utility>
# include <signal.h>
# include <map>
# include <sys/wait.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstdlib>
# include <netdb.h>
# include <sys/socket.h>
# include <sstream>
# include <exception>
# include <iostream>
# include <fstream>
# include <algorithm>
# include <filesystem>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "common.hpp"
# include "Request.hpp"
# include "parseConfig.hpp"
# include "HeaderGen.hpp"
# include "server.hpp"

# define SSTR( x ) static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x ) ).str()



# include "parseConfig.hpp"
# include "HeaderGen.hpp"
# include "server.hpp"

enum    METHOD{
    GET,
    POST,
    DELETE,
    UNKNOWN
};

enum    LOCATION_TYPE{
    EXTENSION,
    FOLDER
};


#endif
