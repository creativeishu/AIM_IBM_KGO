#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <exception>
#include <set>
#include <iostream>

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
  bool operator<(const std::string& in) const {return ((this->name_).compare(in) < 0);}
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
        this->nodes_.push_back(node_type(s, std::vector<double>()));
        this->nodes_.push_back(node_type(v, std::vector<double>()));
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

  std::string query_graph(const std::string& query, const std::size_t depth) const
  {
    const std::size_t index(find_node(query));

    std::set<std::size_t> used_indices;
    std::string sub_graph = "graph G {";
    build_sub_graph(index,depth,used_indices,sub_graph);
    sub_graph += "}";
    return sub_graph;
  }

  void insert_node(const node_type& node)
  {
    nodes_.push_back(node);
  }

private:

  void build_sub_graph(const std::size_t index, const std::size_t depth, std::set<std::size_t>& used_indices, std::string& sub_graph) const
  {
    used_indices.insert(index);
    if(depth > 0)
      for(const auto a : nodes_[index].neighbours_)
        if(used_indices.find(a) != used_indices.end()){
          sub_graph += nodes_[index].name_ + " -- " + nodes_[a].name_ + " [label=contains in]" + ";\n";
          build_sub_graph(a,depth-1,used_indices,sub_graph);
        }
  }

  std::size_t find_node(const std::string& query) const
  {
    const auto it(std::lower_bound(nodes_.begin(),nodes_.end(),query));
    try{
      if(it == nodes_.end())
        throw not_found;
    }
    catch(std::exception& e){
      std::cout << e.what() << '\n';
    }
    return std::distance(nodes_.begin(),it);
  }

  //*******

  std::vector<node_type> nodes_;
};


#endif
