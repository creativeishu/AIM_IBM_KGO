#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <exception>

class not_in_database: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Item not in database";
  }
} not_found;

struct node_type
{

public:

  node_type(const std::string& name, const std::vector<double>& properties)
    : name_(name)
    , properties_(properties)
  {}

  bool operator<(const node_type& in) const {return ((this->name_).compare(in.name_) < 0);}

  std::vector<double> properties_;
  std::string name_;
  std::vector<unsigned> neighbours_;
};

class graph
{

public:

  graph() {};

  graph(const std::string& graph_file)
  {
    //TODO: parse graph file

    std::sort(nodes_.begin(),nodes_.end());
  }

  graph query_graph(const std::string& query, const std::size_t depth)
  {
    const std::size_t index(find_node(query));

    std::set<std::size_t> used_indices;
    graph sub_graph;
    build_sub_graph(index,depth,used_indices,sub_graph);
    return sub_graph;
  }

  void insert_node(const node_type& node)
  {
    nodes_.push_back(node);
  }

private:

  void build_sub_graph(const std::size_t index, const std::size_t depth, std::set<std::size_t>& used_indices, graph& sub_graph)
  {
    sub_graph.insert_node(nodes_[index]);
    used_indices.insert(index);
    if(depth > 0)
      for(const auto a : nodes_[index].neighbours_)
        if(find(used_indices,a) != used_indices.end())
          query_graph(a,depth-1);
  }

  std::size_t find_node(const std::string& query)
  {
    try{
      const auto it(std::lower_bound(nodes_.begin(),nodes_.end(),query));
      if(it == nodes_.end()) throw not_found;
      return std::distance(nodes_.begin(),it);
    }
    catch(std::exception& e){
      std::cout << e.what() << '\n';
    }
  }

  //*******

  std::vector<node_type> nodes_;
};

/*
  binning. nodes are neighbours if values are in the same bin
  const std::size_t num_bins(100);
  std::vector<std::vector<double> > values;
*/

std::string get_query()
{
  std::string query(/*fetch query from web interface*/);
  return query;
}

std::string convert_graph(const graph& sub_graph)
{
  //TODO
}

int main()
{
  const std::size_t depth(3);
  const std::string graph_file("graph.dat");
  const graph G(graph_file);
  const std::string query(get_query());
  const graph sub_graph(G.query_graph(query,depth));

  const std::string sub_graph_str(convert_graph(sub_graph));

  return 0;
}
