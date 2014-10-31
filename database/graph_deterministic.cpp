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
  graph G(graph_file, properties_file);

#if 1
  std::vector<std::pair<std::string, double> > properties;
  properties.push_back( std::make_pair("melting_point", 0.004) );
  G.create_property_edges(properties);
#endif

  double load_time(c.tock());

  const std::string query(argv[1]);
  const std::string property(argv[2]);

  //G.dump_nodes(query, depth, by_name);

  std::cout
    << std::endl
    << "QUERY" << std::endl
    << "=====" << std::endl
    << std::endl
    << "node-name:    " << query << std::endl
    << "property:     " << property << std::endl
    << "depth-limit:  " << depth << std::endl
    << "result-limit: " << limit << std::endl;

  std::cout
    << std::endl
    << "RESULTS" << std::endl
    << "=======" << std::endl
    << std::endl;

  c.tick();
  const auto & paths(G.find_nodes_closest_by_property_comparison(query, depth, by_name, property, limit));
  std::vector<size_t> nodes; // needed to reuse the error calculation function
  double search_time(c.tock());

  for (auto p: paths)
  {
    nodes.push_back(p.second.back());
    const auto & n(G.get(p.second.back()));
    const double v(std::stod(n.find_property(property)->second));
    std::cout << n.name_ << ": " <<  v << " (delta: " << p.first << ")" << std::endl;
    std::cout << "  " << query;
    for (auto i(p.second.begin()+1); i != p.second.end(); ++i)
      std::cout << " -> " << G.get(*i).name_ << " (" << G.get(*i).id_ << ")";
    std::cout << std::endl;
  }

  std::cout
    << std::endl
    << "TIMING" << std::endl
    << "======" << std::endl
    << std::endl
    << "loading [s]: " << load_time << std::endl
    << "search [s]:  " << search_time << std::endl;

  std::cout
    << std::endl
    << "ERRORS" << std::endl
    << "======" << std::endl
    << std::endl
    << "MAE: "  << G.compute_error(query, property, by_name, nodes) << std::endl
    << std::endl;

  return 0;
}
