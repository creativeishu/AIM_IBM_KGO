#include "graph.hpp"

#include <iostream>

int main()
{
  const std::string graph_file("../../edges_test.txt");
  const std::string properties_file("../../props_test.txt");

  const bool undirected(true);

  graph G(graph_file,properties_file,undirected);

  // const std::vector<std::pair<std::string,double> > properties({
  //     std::make_pair("melting_point",0.05)
  //       });
  // G.create_property_edges(properties);

  const std::string query("/m/02kcv_c");
  // const std::string query("Iron");

  const bool by_name(false);

  //const std::string sub_graph(G.query_graph(query,depth,by_name));
  //std::cout << sub_graph << std::endl;
  
  std::cout << "Total size : " << G.count() << std::endl;
  
  std::size_t ix = G.find_node_id(query);
  std::cout << "Found " << ix << std::endl;
  std::cout << "It is : " << G.get(ix).name_ << std::endl;
  
  const std::string property("melting_point");

  const std::size_t N(10);

  const auto indices_exact(G.query_graph_exact(query,N,property,by_name));

  for(const auto a : indices_exact)
    std::cout << G.get(a).name_ << ' ' << std::stod(G.get(a).find_property(property)->second) << std::endl;

  const double delta_exact(G.compute_error(query,property,by_name,indices_exact));

  std::cout << "delta exact: " << delta_exact << std::endl;

  std::cout << "----------" << std::endl;

  // const std::size_t depth(32);

  // for(std::size_t depth = 4; depth <= 8192; depth *= 2){

  //   const auto indices1(G.query_graph_parallel(query,depth,N,property,by_name));

  //   const double delta(G.compute_error(query,property,by_name,indices1));

  //   std::cout << depth << ' ' << delta << std::endl;

  //   // for(const auto a : indices1)
  //   //   std::cout << G.get(a).name_ << ' ' << G.get(a).find_property(property)->second << std::endl;

  // }

  // std::cout << "----------" << std::endl;

  // const auto indices0(G.query_graph_random(query,depth,N,property,by_name));
  // for(const auto a : indices0)
  //   std::cout << G.get(a).name_ << ' ' << G.get(a).find_property(property)->second << std::endl;

  return 0;
}
