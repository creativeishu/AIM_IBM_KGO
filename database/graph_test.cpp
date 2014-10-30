#include "graph.hpp"

#include <iostream>

int main()
{
  const std::size_t depth(3);
  const std::string graph_file("../../edges_test.txt");
  const std::string properties_file("../../props_test.txt");

  graph G(graph_file,properties_file);

  const std::vector<std::pair<std::string,double> > properties({
      std::make_pair("melting_point",0.05)
        });
  G.create_property_edges(properties);

  const std::string query("/m/03cl_tj");
  //const std::string query("Iron");

  const bool by_name(false);

  //const std::string sub_graph(G.query_graph(query,depth,by_name));
  //std::cout << sub_graph << std::endl;
  
  std::cout << "Total size : " << G.count() << std::endl;
  
  std::size_t ix = G.find_node_id(query);
  std::cout << "Found " << ix << std::endl;
  std::cout << "It is : " << G.get(ix).name_ << std::endl;
  
  const std::string property("melting_point");
  const double threshold(120.0);

  const auto indices(G.query_graph_parallel(query,depth,property,threshold,by_name));

  for(const auto a : indices)
    std::cout << G.get(a).name_ << ' ' << G.get(a).find_property(property)->second << std::endl;

  return 0;
}
