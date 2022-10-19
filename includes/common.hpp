template < class T >
struct greater
{
    bool operator()( const T & a, const T & b ) const
    {
        return a > b;
    }
};


std::list<std::string> ft_split( const std::string & header, const std::string & charset );
std::list<std::string> ft_split_no_r( const std::string & header, const std::string & charset );
std::string trim_data( std::string & raw_data, const std::string & data_name );
std::string trim_data_semicolon( std::string & raw_data, const std::string & data_name );
std::string 	insertion_semicolon( std::string & line, const std::string & name );
std::string	 insertion( std::string & line, const std::string & name );
bool         file_already_exist( const std::string & path );
bool         isonly_digit( const std::string & haystack );
bool         is_folder( std::string & URI );