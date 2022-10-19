#include "../includes/parseConfig.hpp"

std::list<std::string> parseConfig::ft_split( const std::string & header, const std::string & charset )
{
	size_t 					start = 0, end, charset_len = charset.length();
	std::string				tmp;
	std::list<std::string> 	ret;

	while ((end = header.find(charset, start)) != std::string::npos)
	{
		tmp = header.substr(start, 	end - start);
		start = end + charset_len;
		ret.push_back(tmp);
	}
	ret.push_back(header.substr(start));

	return ret;		
}