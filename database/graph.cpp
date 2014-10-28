#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <exception>
#include <set>

#include "parse.hpp"

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
  bool operator==(const node_type& in) const {return this->name_ == in.name_;}

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
    EdgeFile ef(graph_file);

    std::vector<std::pair<std::string,std::string> > edges;
    ef.load([this, &edges] (const std::string & s, const std::string &, const std::string &v) {
      this->nodes_.push_back(node_type(s), std::vector<double>());
      this->nodes_.push_back(node_type(v), std::vector<double>());
      edges.push_back(std::make_pair(s,v));
      return true;
    });

    std::set<node_type> nodes_set(nodes_.begin(),nodes_.end());
    nodes_.assign(nodes_set.begin(),nodes_set.end());
    std::sort(nodes_.begin(),nodes_.end());

    for(const auto& edge : edges){
      const std::size_t ind0(find_node(edge.first));
      const std::size_t ind1(find_node(edge.second));
      nodes_[ind0].neighbours_.push_back(ind1);
    }
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

  std::string convert_graph() const
  {
    //TODO

    std::vector<std::pair<std::size_t,std::size_t> > edges;

    // for(const auto a )

    const auto edges(sub_graph.get_edges());

    std::string out;
    for(const auto& edge : edges)
      out += ;
    
    return out;
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

int main()
{
  const std::size_t depth(3);
  const std::string graph_file("graph.dat");
  const graph G(graph_file);
  const std::string query(get_query());
  const graph sub_graph(G.query_graph(query,depth));

  const std::string sub_graph_str(sub_graph.convert_graph());

  return 0;
}
