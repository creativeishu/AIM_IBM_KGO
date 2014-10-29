#include "graph.hpp"

#include <iostream>

int main()
{
  const std::size_t depth(3);
  const std::string graph_file("../../edges_test.txt");
  const std::string properties_file("../../props_test.txt");
  const graph G(graph_file,properties_file);

  //const std::string query("/m/0lbt3");
  const std::string query("Iron");

  const bool by_name(true);
  const std::string sub_graph(G.query_graph(query,depth,by_name));

  std::cout << sub_graph << std::endl;

  return 0;
}
