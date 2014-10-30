#include "graph.hpp"

#include <cstdlib>
#include <iostream>

void print_usage(const char * argv0, int exit_value)
{
  std::cout << "Usage: " << argv0 << " <node-name> <property-name> <depth-limit> <result-limit>" << std::endl;
  exit(exit_value);
}

const std::string graph_file("../../edges_test.txt");
const std::string properties_file("../../props_test.txt");
const bool by_name(true);

int main(int argc, char *argv[])
{
  if (argc != 5)
    print_usage(argv[0], 1);

  std::size_t depth(0), limit(0);
  try
  {
    depth = std::stoul(argv[3]);
    limit = std::stoul(argv[4]);
  }
  catch (const std::invalid_argument & e)
  {
    print_usage(argv[0], 2);
  }

  ClockMeasure c;

  c.tick();
  const graph G(graph_file, properties_file);
  double load_time(c.tock());

  const std::string query(argv[1]);
  const std::string property(argv[2]);

  std::cout << std::endl;
  std::cout << "QUERY" << std::endl;
  std::cout << "=====" << std::endl << std::endl;

  std::cout << "node-name:    " << query << std::endl;
  std::cout << "property:     " << property << std::endl;
  std::cout << "depth-limit:  " << depth << std::endl;
  std::cout << "result-limit: " << limit << std::endl;

  std::cout << std::endl;
  std::cout << "RESULTS" << std::endl;
  std::cout << "=======" << std::endl << std::endl;

  c.tick();
  const auto & nodes(G.find_nodes_closest_by_property_comparison(query, depth, by_name, property, limit));
  double search_time(c.tock());

  for (auto e: nodes)
  {
    const auto & n(G.get(e.second));
    const double v(std::stod(n.find_property(property)->second));
    std::cout << n.name_ << ": " <<  v << " (delta: " << e.first << ")" << std::endl;
  }

  std::cout << std::endl;
  std::cout << "TIMING" << std::endl;
  std::cout << "======" << std::endl << std::endl;
  std::cout << "loading: " << load_time << std::endl;
  std::cout << "search:  " << search_time << std::endl;
  return 0;
}
