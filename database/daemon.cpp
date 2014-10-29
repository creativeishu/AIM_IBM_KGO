#include <iostream>
#include <fstream>
#include "graph.hpp"


// Path to the pipe used for communication (create with "mkfifo pipe_path" and set permissions right)
static const char* pipe_path = "/tmp/aim2014_graph_pipe";


int main()
{
    const std::size_t depth(3);
    const std::string graph_file("graph.dat");
    const graph G(graph_file);
    const std::string query("Copper");

    while(1)
    {
        // Get the query
        std::string t;
        std::ifstream ifs(pipe_path);
        ifs >> t;
        ifs.close();
        std::cout << "Received " << t << std::endl;

        // Reply to query
        const std::string res = G.query_graph(query,depth);
        std::ofstream ofs(pipe_path);
        ofs << res;
        ofs.close();
        std::cout << "Sent     " << res << std::endl;
    }
    return 0;
}
