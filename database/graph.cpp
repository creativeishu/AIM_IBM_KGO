#include "graph.hpp"

#include <iostream>
#include <tuple>
#include <regex>
#include <stack>

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

  const std::vector<std::pair<std::string,double> > properties({
      std::make_pair("melting_point",0.05)
        });

  for(const auto& a : properties)
    add_similarity(a.first,a.second);

  //----------

  // for(const auto a : vec)
  //   std::cout << a.index << ' ' << a.value << std::endl;

  // for(const auto& node : nodes_){
  //   for(const auto a : node.properties_)
  //     std::cout << a << std::endl;
  //   std::cout << "***********" << std::endl;
  //   std::cin.get();
  // }

  // std::cout << nodes_.size() << ' ' << edges.size() << std::endl; exit(0);

  // std::vector<std::size_t> dist(nodes_.size(),0);
  // for(const auto a : nodes_)
  //   ++dist[a.neighbours_.size()];

  // for(unsigned i = 0; i < dist.size(); ++i)
  //   if(dist[i] > 0)
  //     std::cout << i << ' ' << dist[i] << std::endl;

  // for(const auto& node : nodes_)
  //   std::cout << node.id_ << " | " << node.name_ << std::endl;

  // exit(0);

  //----------
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
  for(const auto ind0 : indices0) {
    sub_graph += nodes_[ind0].id_;
    sub_graph += " [";
    sub_graph += "label=\"" + nodes_[ind0].name_ + "\"";
        
    std::string title_str = "";
    node_type::PropertyContainer::const_iterator match;
        
    match = nodes_[ind0].properties_.find("measurement./chemistry/chemical_element/melting_point");
    if (match != nodes_[ind0].properties_.end())
      title_str += "Melting T : " + match->second + " °C<br />";
    match = nodes_[ind0].properties_.find("measurement./chemistry/chemical_element/boiling_point");
    if (match != nodes_[ind0].properties_.end())
      title_str += "Boiling T : " + match->second + " °C<br />";
    match = nodes_[ind0].properties_.find("measurement./chemistry/chemical_compound/melting_point");
    if (match != nodes_[ind0].properties_.end())
      title_str += "Melting T : " + match->second + " °C<br />";
    match = nodes_[ind0].properties_.find("measurement./chemistry/chemical_compound/boiling_point");
    if (match != nodes_[ind0].properties_.end())
      title_str += "Boiling T : " + match->second + " °C<br />";
    match = nodes_[ind0].properties_.find("measurement./chemistry/chemical_element/atomic_mass");
    if (match != nodes_[ind0].properties_.end())
      title_str += "Mass : " + match->second + "u<br />";
    match = nodes_[ind0].properties_.find("measurement./chemistry/chemical_compound/atomic_mass");
    if (match != nodes_[ind0].properties_.end())
      title_str += "Mass : " + match->second + "u<br />";
    match = nodes_[ind0].properties_.find("measurement./chemistry/isotope/mass");
    if (match != nodes_[ind0].properties_.end())
      title_str += "Mass : " + match->second + "u<br />";
        
        
    if (!title_str.empty())
      sub_graph += " title=\""+ title_str + "\"";
        
    sub_graph += "];\n";
  }
  if(depth > 0){
        
    std::vector<std::size_t> indices1;
    for (const auto ind0 : indices0)
      for (const auto b : nodes_[ind0].neighbours_)
      {
        const std::size_t ind1(b.first);
        if (used_indices.find(ind1) == used_indices.end())
        {
          const std::string label(b.second);
          used_indices.insert(ind1);
          sub_graph += nodes_[ind0].id_ + " -- " + nodes_[ind1].id_ + " [label=\"" + label + "\"];\n";
          indices1.push_back(ind1);
        }
      }
        
    build_sub_graph(indices1,depth-1,used_indices,sub_graph);
  }
    
}

void graph::add_similarity(const std::string property, const double threshold)
{
  std::vector<prop_type> vec;
  for(std::size_t ind = 0; ind < nodes_.size(); ++ind)
    for(const auto a : nodes_[ind].properties_)
      if(std::regex_match (a.first, std::regex("(.*)(" + property + ")(.*)"))){
        vec.push_back(prop_type(ind,std::stod(a.second)));
        break;
      }

  std::sort(vec.begin(),vec.end(),std::less<prop_type>());

  const double min_p(vec[0].value);
  const double max_p(vec[vec.size()-1].value);

  const double offset(0.0-min_p);
  const double factor(1.0/(max_p+offset));

  for(auto& a : vec)
    a.value = (a.value+offset)*factor;

  for(std::size_t i = 0; i < vec.size(); ++i){

    const std::size_t ind0(vec[i].index);
    const double value0(vec[i].value);
    long j;

    j = long(i)-1;
    while(j >= 0 && std::fabs(vec[std::size_t(j)].value - value0) < threshold){
      nodes_[ind0].neighbours_.push_back(std::make_pair(vec[std::size_t(j)].index,property));
      --j;
    }

    j = long(i)+1;
    while(j < long(vec.size()) && std::fabs(vec[std::size_t(j)].value - value0) < threshold){
      nodes_[ind0].neighbours_.push_back(std::make_pair(vec[std::size_t(j)].index,property));
      ++j;
    }

  }
}

void graph::visit_nodes_bfs(
  const std::size_t root,
  std::function<bool (const node_type &)> f,
  const std::size_t depth) const
{
  std::set<size_t> visited({ root });
  std::stack<std::pair<size_t,size_t>> queue({ std::make_pair(root, 0) });

  while (!queue.empty())
  {
    size_t t(0), d(0);
    std::tie(t, d) = queue.top();
    queue.pop();

    // call the function on the node and terminate if it returns false
    if (!f(nodes_[t]))
      return;

    // do not add further neighbours since we reached maximum depth already
    if (d >= depth)
      continue;

    for (size_t n(0); n < nodes_[t].neighbours_.size(); ++n)
    {
      size_t n_i(nodes_[t].neighbours_[n].first);
      if (visited.find(n_i) == visited.end())
      {
        visited.emplace(n_i);
        queue.emplace(n_i, d+1);
      }
    }
  }
}

void graph::dump_nodes(const std::string& query, const std::size_t depth, const bool by_name) const
{
  const std::size_t index(by_name ? find_node_name(query) : find_node_id(query));
  if(index == nodes_.size())
    return;

  auto printer = [](const node_type & node) {
    std::cout << node.id_ << " (name: " << node.name_ << ")" << std::endl;
    return true;
  };
  visit_nodes_bfs(index, printer, depth);
}

std::size_t graph::find_node(const std::string& query) const
{
  const auto it(std::lower_bound(nodes_.begin(),nodes_.end(),query));
  return static_cast<std::size_t>(std::distance(nodes_.begin(),it));
}

std::size_t graph::find_node_id(const std::string& query) const
{
  const auto it(lookup_id_.find(query));
  return (it == lookup_id_.end() ? nodes_.size() : it->second);
}

std::size_t graph::find_node_name(const std::string& query) const
{
  const auto it(lookup_name_.find(query));
  return (it == lookup_name_.end() ? nodes_.size() : it->second);
}
