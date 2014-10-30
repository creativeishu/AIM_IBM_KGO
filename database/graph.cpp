#include "graph.hpp"

#include <iostream>
#include <tuple>
#include <regex>
#include <stack>
#include <random>
#include <omp.h>

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
    // lookup_id_.insert(std::make_pair(nodes_[i].id_,i));
    lookup_id_.emplace(nodes_[i].id_,i);

  PropertiesFile pf(properties_file);
  pf.load([this](const std::string & s, PropertiesFile::PropertyContainer && props){
      const std::size_t ind(find_node_id(s));

      if(ind == nodes_.size())
        return true;

      for(const auto a : props)
        if(std::regex_match (a.first, std::regex("(name)(.*)"))){
          const std::string name(a.second);
          nodes_[ind].name_ = name;
          // lookup_name_.insert(std::make_pair(name,ind));
          lookup_name_.emplace(name,ind);
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
}

void graph::create_property_edges(const std::vector<std::pair<std::string, double> > properties)
{
  for(const auto& a : properties)
    add_similarity(a.first,a.second);
}

std::string graph::query_graph(const std::string& query, const std::size_t depth, const bool by_name) const
{
  const std::size_t index(by_name ? find_node_name(query) : find_node_id(query));
  std::string sub_graph = "graph G {\n";
  if(index == nodes_.size())
    sub_graph += "nf [label=\"NOT FOUND\"];\n";
  else{
    std::set<std::size_t> indices_tot({index});
    std::vector<std::size_t> indices0({index});

    for(std::size_t d = 0; d <= depth; ++d){

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

      if(d < depth){
        std::vector<std::size_t> indices1;
        for(const auto ind0 : indices0)
          for(const auto b : nodes_[ind0].neighbours_){
            const std::size_t ind1(b.first);
            if(indices_tot.find(ind1) == indices_tot.end()){
              const std::string label(b.second);
              indices_tot.insert(ind1);
              sub_graph += nodes_[ind0].id_ + " -- " + nodes_[ind1].id_ + " [label=\"" + label + "\"];\n";
              indices1.push_back(ind1);
            }
          }

        std::swap(indices0,indices1);
      }

    }

  }

  sub_graph += "}";
  return sub_graph;
}

void graph::query_graph_parallel(const std::string& query, const std::size_t depth, const std::string property, const double threshold, const bool by_name) const
{
  const std::size_t index(by_name ? find_node_name(query) : find_node_id(query));

  if(index == nodes_.size()){
    std::cout << "NODE NOT FOUND" << std::endl;
    return;
  }

  double value0;
  bool found(false);
  for(const auto a : nodes_[index].properties_)
    if(std::regex_match (a.first, std::regex("(.*)(" + property + ")(.*)"))){
      value0 = std::stod(a.second);
      found = true;
      break;
    }

  if(!found){
    std::cout << "NODE PROPERTY NOT FOUND" << std::endl;
    return;
  }

  std::size_t num_threads;
#pragma omp parallel
  {
    num_threads = omp_get_num_threads();
  }

  std::vector<std::set<std::size_t> > nodes_tot(num_threads);

#pragma omp parallel
  {
    const unsigned thread_num(omp_get_thread_num());

    std::mt19937 generator(thread_num);
    const std::function<std::size_t()> rng(std::bind(std::uniform_int_distribution<std::size_t>(0, nodes_.size()), std::ref(generator)));

    auto& nodes_loc(nodes_tot[thread_num]);

    nodes_loc.insert(index);
    std::size_t ind0(index);
    for(std::size_t d = 0; d <= depth; ++d){
      const std::size_t ns(nodes_[ind0].neighbours_.size());
      if(ns > 0){
        const std::size_t rand_n(rng()%ns);
        const std::size_t ind1(nodes_[ind0].neighbours_[rand_n].first);

        double value1;
        bool found(false);
        for(const auto a : nodes_[ind1].properties_)
          if(std::regex_match (a.first, std::regex("(.*)(" + property + ")(.*)"))){
            value1 = std::stod(a.second);
            found = true;
            break;
          }

        if(std::fabs(value1 - value0) < threshold)
          nodes_loc.insert(ind1);

        ind0 = ind1;
      }
    }

  }

  std::set<std::size_t> nodes_tot_set;
  for(const auto& a : nodes_tot)
    std::copy( a.begin(), a.end(), std::inserter( nodes_tot_set, nodes_tot_set.end() ) );

  for(const auto a : nodes_tot_set)
    std::cout << nodes_[a].name_ << ' ' << nodes_[a].get_property(property)->second << std::endl;
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
                            std::function<bool (size_t)> f,
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
      if (!f(t))
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

  auto printer = [this](size_t node_index) {
    std::cout << nodes_[node_index].id_ << " (name: " << nodes_[node_index].name_ << ")" << std::endl;
    return true;
  };
  visit_nodes_bfs(index, printer, depth);
}

std::map<double,size_t> graph::find_nodes_closest_by_property_comparison(
      const std::string& query,
      const std::size_t depth,
      const bool by_name,
      const std::string & property,
      const std::size_t limit) const
{
  std::map<double,size_t> nodes_found;

  const std::size_t index(by_name ? find_node_name(query) : find_node_id(query));
  if(index == nodes_.size())
    return nodes_found;

  // check that at least the root node has a matching property and use that one
  const auto ref_p_i(nodes_[index].get_property(property));

  if (ref_p_i == nodes_[index].properties_.end())
    return nodes_found;

  const double ref(stod(ref_p_i->second));

  auto f = [this,ref,&nodes_found,&property,&limit](size_t n_i) {
    // get an iterator to the wanted property
    const auto p_i(nodes_[n_i].get_property(property));

    // ignore the node if the property does not exist
    if (p_i == nodes_[n_i].properties_.end())
      return true;

    // do a sorted insert
    const double v(std::stod(p_i->second));
    nodes_found.emplace(std::abs(ref - v), n_i);

    // reduce the length of the list to limit+1 (since the root node will always be part of it)
    if (nodes_found.size() > (limit+1))
      nodes_found.erase(--nodes_found.end());

    // continue with the search
    return true;
  };

  visit_nodes_bfs(index, f, depth);

  return nodes_found;
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
