#include "../includes/includes.hpp"

std::list<std::string> ft_split( const std::string & header, const std::string & charset )
{
	size_t start = 0, end, charset_len = charset.length();
	std::string tmp;
	std::list<std::string> ret;
	while ((end = header.find(charset, start)) != std::string::npos)
	{
		tmp = header.substr(start, 	end - start);
		start = end + charset_len;
		ret.push_back(tmp);
	}
	ret.push_back(header.substr(start));
	return ret;
}

std::list<std::string> ft_split_no_r( const std::string & header, const std::string & charset )
{
	size_t i = 0;

	std::list<std::string> ret;

	size_t cursor = header.find_first_of(charset);
	
	while ( cursor != std::string::npos )
	{
		if ( cursor == i )
			i = ++cursor;
		else
		{
			ret.push_back(header.substr(i, cursor - i));
			i = ++cursor;
		}
		cursor = header.find_first_of(charset, cursor);
	}
	if ( i != header.length() )
		ret.push_back(header.substr(i, header.length()));
	return ret;
}


std::string trim_data( std::string & raw_data, const std::string & data_name )
{
 	if ( raw_data.find(data_name) != std::string::npos )
		raw_data.erase(raw_data.find(data_name), data_name.size());
	
	if ( raw_data.find(";") != std::string::npos )
		raw_data.erase(raw_data.find(";"));
	
	std::string::iterator it = raw_data.begin();
	while ( it != raw_data.end() && isspace(*it) )
		it++;
	
	std::string::reverse_iterator rit = raw_data.rbegin();
	while ( rit != raw_data.rend() && isspace(*rit) )
		rit++;

	std::string::iterator trim_end = rit.base();

	if ( std::distance(it, trim_end) < 0 )
		return "";

	std::string trimmed_raw_data(it, trim_end);

	while ( trimmed_raw_data.find("\t") != std::string::npos )
		trimmed_raw_data[trimmed_raw_data.find("\t")] = ' ';

	return trimmed_raw_data;
}

std::string trim_data_semicolon( std::string & raw_data, const std::string & data_name )
{
 	if ( raw_data.find(data_name) != std::string::npos )
		raw_data.erase(raw_data.find(data_name), data_name.size());
	
	std::string::iterator it = raw_data.begin();
	while ( it != raw_data.end() && isspace(*it) )
		it++;
	
	std::string::reverse_iterator rit = raw_data.rbegin();
	while ( rit != raw_data.rend() && isspace(*rit) )
		rit++;

	std::string::iterator trim_end = rit.base();

	if ( std::distance(it, trim_end) < 0 )
		return "";

	std::string trimmed_raw_data(it, trim_end);

	while ( trimmed_raw_data.find("\t") != std::string::npos )
		trimmed_raw_data[trimmed_raw_data.find("\t")] = ' ';

	return trimmed_raw_data;
}

std::string	 insertion( std::string & line, const std::string & name )
{
	return trim_data(line, name);
}

std::string 	insertion_semicolon( std::string & line, const std::string & name )
{
	return trim_data_semicolon(line, name);
}

//? return true if the file exist, else false
bool         file_already_exist( const std::string & path )
{
	struct stat buf;
	if ( stat(path.c_str(), &buf) != -1 )
		return true;
	return false;
}

bool         isonly_digit( const std::string & haystack )
{
	for ( std::string::size_type i = 0 ; i < haystack.size() ; i++ )
		if ( !isdigit(haystack[i]) )
			return false;
	return true;
}

bool         is_folder( std::string & URI )
{
	if ( URI == "/" )
		return true;

	if ( URI.size() >= 2 && URI[0] == '/' )
		URI.erase(0, 1);
	
	DIR *dir = opendir(URI.c_str());
	if ( dir == NULL )
		return false;
	closedir(dir);
	return true;
}