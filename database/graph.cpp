#include "graph.hpp"

#include <iostream>
#include <tuple>
#include <regex>

graph::graph(const std::string& graph_file, const std::string& properties_file)
{
  EdgeFile ef(graph_file);

  std::vector<std::tuple<std::string,std::string,std::string> > edges;
  ef.load([this, &edges] (const std::string & s, const std::string & p, const std::string &v) {
      this->nodes_.push_back(node_type(s));
      this->nodes_.push_back(node_type(v));
      edges.push_back(std::make_tuple(s,p,v));
      return true;
    });

  std::set<node_type> nodes_set(nodes_.begin(),nodes_.end());
  nodes_.assign(nodes_set.begin(),nodes_set.end());
  // std::sort(nodes_.begin(),nodes_.end());

  for(std::size_t i = 0; i < nodes_.size(); ++i)
    lookup_id_.insert(std::make_pair(nodes_[i].id_,i));

  PropertiesFile pf(properties_file);
  pf.load([this](const std::string & s, PropertiesFile::PropertyContainer && props){
      const std::size_t ind(find_node_id(s));

      for(const auto a : props)
        if(std::regex_match (a.first, std::regex("(name)(.*)"))){
          const std::string name(a.second);
          nodes_[ind].name_ = name;
          lookup_name_.insert(std::make_pair(name,ind));          
          break;
        }

      nodes_[ind].properties_ = props;
      return true;
    });

  for(const auto& edge : edges){
    const std::size_t ind0(find_node_id(std::get<0>(edge)));
    const std::size_t ind1(find_node_id(std::get<2>(edge)));
    nodes_[ind0].neighbours_.push_back(std::make_pair(ind1,std::get<1>(edge)));
  }

  // for(const auto& node : nodes_){
  //   for(const auto a : node.properties_)
  //     std::cout << a << std::endl;
  //   std::cout << "***********" << std::endl;
  //   std::cin.get();
  // }

  // exit(0);

  //TODO: additional edges based on similatiry of properties

  // std::cout << nodes_.size() << ' ' << edges.size() << std::endl; exit(0);

  // std::vector<std::size_t> dist(nodes_.size(),0);
  // for(const auto a : nodes_)
  //   ++dist[a.neighbours_.size()];

  // for(unsigned i = 0; i < dist.size(); ++i)
  //   if(dist[i] > 0)
  //     std::cout << i << ' ' << dist[i] << std::endl;

  // exit(0);

  // for(const auto& node : nodes_)
  //   std::cout << node.id_ << " | " << node.name_ << std::endl;

  // exit(0);
}

std::string graph::query_graph(const std::string& query, const std::size_t depth, const bool by_name) const
{
  const std::size_t index(by_name ? find_node_name(query) : find_node_id(query));
  std::string sub_graph = "graph G {\n";
  if(index == nodes_.size())
    sub_graph += "nf [label=\"NOT FOUND\"];\n";
  else{
    std::set<std::size_t> used_indices({index});
    build_sub_graph({index},depth,used_indices,sub_graph);
  }

  sub_graph += "}";
  return sub_graph;
}

void graph::build_sub_graph(const std::vector<std::size_t>& indices0, const std::size_t depth, std::set<std::size_t>& used_indices, std::string& sub_graph) const
{
  for(const auto ind0 : indices0)
    sub_graph += nodes_[ind0].id_ + " [label=\"" + nodes_[ind0].name_ + "\"];\n";

  if(depth > 0){

    std::vector<std::size_t> indices1;
    for(const auto ind0 : indices0)
      for(const auto b : nodes_[ind0].neighbours_){
        const std::size_t ind1(b.first);
        if(used_indices.find(ind1) == used_indices.end()){
          const std::string label(b.second);
          used_indices.insert(ind1);
          sub_graph += nodes_[ind0].id_ + " -- " + nodes_[ind1].id_ + " [label=\"" + label + "\"];\n";
          indices1.push_back(ind1);
        }
      }

    build_sub_graph(indices1,depth-1,used_indices,sub_graph);
  }
        
}

std::size_t graph::find_node(const std::string& query) const
{
  const auto it(std::lower_bound(nodes_.begin(),nodes_.end(),query));
  return static_cast<std::size_t>(std::distance(nodes_.begin(),it));
}

std::size_t graph::find_node_id(const std::string& query) const
{
  return lookup_id_.at(query);
}

std::size_t graph::find_node_name(const std::string& query) const
{
  return lookup_name_.at(query);
}
