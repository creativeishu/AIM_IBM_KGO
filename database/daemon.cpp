#include <iostream>
#include <fstream>
#include "graph.hpp"


// Path to the pipe used for communication (create with "mkfifo pipe_path" and set permissions right)
static const char* pipe_path = "/tmp/aim2014_graph_pipe";


int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "Usage: "<< argv[0] << " <graph_file> <properties_file>" << std::endl;
        return -1;
    }
    const std::size_t depth(1);
    const std::string graph_file(argv[1]);
    const std::string properties_file(argv[2]);
    const graph G(graph_file,properties_file);

    while(1)
    {
        // Get the query
        std::string query;
        std::ifstream ifs(pipe_path);
        ifs >> query;
        ifs.close();
        std::cout << "Received " << query << std::endl;

        // Reply to query
        const bool by_name(false);
        const std::string res = G.query_graph(query,depth,by_name);
        std::ofstream ofs(pipe_path);
        ofs << res;
        ofs.close();
        std::cout << "Sent     " << res << std::endl;
    }
    return 0;
}
