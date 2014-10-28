#include "graph.hpp"

#include <iostream>

int main()
{
  const std::size_t depth(3);
  const std::string graph_file("graph.dat");
  const graph G(graph_file);

  const std::string query("Copper");

  const std::string sub_graph(G.query_graph(query,depth));

  std::cout << sub_graph << std::endl;

  return 0;
}
