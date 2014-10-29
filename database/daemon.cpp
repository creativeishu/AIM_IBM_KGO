#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>


// Path to the pipe used for communication (create with "mkfifo pipe_path" and set permissions right)
static const char* pipe_path = "/tmp/aim2014_graph_pipe";


std::string get_graph(std::string const& query)
{

    std::string res("digraph finite_state_machine { rankdir=LR;\n size=\"8,5\" node [shape = doublecircle];\n LR_0 LR_3 LR_4 LR_8;\n node [shape = circle];\n LR_0 -> LR_2 [ label = \"SS(B)\" ];\n LR_0 -> LR_1 [ label = \"SS(S)\" ];\n LR_1 -> LR_3 [ label = \"S($end)\" ];\n LR_2 -> LR_6 [ label = \"SS(b)\" ];\n LR_2 -> LR_5 [ label = \"SS(a)\" ];\n LR_2 -> LR_4 [ label = \"S(A)\" ];\n LR_5 -> LR_7 [ label = \"S(b)\" ];\n LR_5 -> LR_5 [ label = \"S(a)\" ];\n LR_6 -> LR_6 [ label = \"S(b)\" ];\n LR_6 -> LR_5 [ label = \"S(a)\" ];\n LR_7 -> LR_8 [ label = \"S(b)\" ];\n LR_7 -> LR_5 [ label = \"S(a)\" ];\n LR_8 -> LR_6 [ label = \"S(b)\" ];\n LR_8 -> LR_5 [ label = \"S(a)\" ];\n}\n");
    return res;
}

int main()
{
    while(1)
    {
        // Get the query
        std::string t;
        std::ifstream ifs(pipe_path);
        ifs >> t;
        ifs.close();
        std::cout << "Received " << t << std::endl;

        // Reply to query
        std::string res = get_graph(t);
        std::ofstream ofs(pipe_path);
        ofs << res;
        ofs.close();
        std::cout << "Sent     " << res << std::endl;
    }
    return 0;
}
