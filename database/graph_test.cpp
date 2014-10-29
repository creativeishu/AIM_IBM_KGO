#include "graph.hpp"

#include <iostream>

int main()
{
  const std::size_t depth(3);
  const std::string graph_file("../edges_test.txt");
  const graph G(graph_file);

  const std::string query("/m/0hqtqs5");

  const std::string sub_graph(G.query_graph(query,depth));

  std::cout << sub_graph << std::endl;

  return 0;
}
