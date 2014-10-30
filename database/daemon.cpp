#include <iostream>
#include <fstream>
#include "graph.hpp"


// Path to the pipe used for communication (create with "mkfifo pipe_path" and set permissions right)
static const char* pipe_path = "/tmp/aim2014_graph_pipe";

inline std::string trim(std::string& str)
{
  str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
  str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
  return str;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "Usage: "<< argv[0] << " <graph_file> <properties_file>" << std::endl;
        return -1;
    }
    const std::size_t depth(2);
    const std::string graph_file(argv[1]);
    const std::string properties_file(argv[2]);
    const graph G(graph_file,properties_file);

    while(1)
    {
        // Get the query
        bool by_name;
        std::string query;
        std::ifstream ifs(pipe_path);
        ifs >> by_name;
        getline(ifs, query);
        ifs.close();
        trim(query);
        std::cout << "By name  " << by_name << std::endl;
        std::cout << "Received " << query << std::endl;

        // Reply to query
        // const bool by_name(false);
        const std::string res = G.query_graph(query,depth,by_name);
        std::ofstream ofs(pipe_path);
        ofs << res;
        ofs.close();
        std::cout << "Sent     " << res << std::endl;
    }
    return 0;
}
