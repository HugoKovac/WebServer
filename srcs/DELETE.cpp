#include "../includes/includes.hpp"

void	Server::treat_DELETE_request( struct header & head, id_server_type &server_id )
{
    std::string location_name = retrieve_location_name(head.path, server_id);
    std::string upload_path;

    if ( confs[server_id].locations.find(location_name) != confs[server_id].locations.end() )
        upload_path = confs[server_id].locations[location_name].upload_path;
    else
        upload_path = confs[server_id].locations["/"].upload_path;

    std::string file_path = upload_path + "/" + targetLocation(head.path, server_id);

    while ( file_path.find("//") != std::string::npos )
        file_path.erase(file_path.find("//"), 1);

    std::cout << RED << "[" << file_path << "]" << WHITE << std::endl;

    if ( file_already_exist(file_path) == true )
    {
        if ( unlink(file_path.c_str()) == -1 )
        {
            send_500(server_id);
            return ;
        }
    }
    send_202();
}