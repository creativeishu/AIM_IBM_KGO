#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "parse.hpp"
#include "utility.hpp"

#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <map>
#include <functional>
#include <regex>

class graph
{
  struct node_type
  {
    typedef std::unordered_map<std::string, std::string> PropertyContainer;

    node_type(const std::string& id)
      : id_(id)
      , properties_(PropertyContainer())
      , name_("nolabel")
    {}

    bool operator<(const node_type& in) const {return ((this->id_).compare(in.id_) < 0);}
    bool operator<(const std::string& in) const {return ((this->id_).compare(in) < 0);}
    bool operator==(const node_type& in) const {return this->id_ == in.id_;}

    PropertyContainer::const_iterator find_property(const std::string & property) const
    {
      const auto prop_match(std::regex(".*" + property + ".*"));
      return std::find_if(
        properties_.begin(),
        properties_.end(),
        [&prop_match](const std::pair<std::string, std::string> & p) { return std::regex_match(p.first, prop_match); }
        );
    }

    bool search_neighbour(const std::size_t in) const
    {
      for(const auto& a : neighbours_)
        if(a.first == in)
          return true;
      return false;
    }

    //*******

    std::string id_;
    PropertyContainer properties_;
    std::string name_;
    std::vector<std::pair<std::size_t,std::string> > neighbours_;
  };

  struct prop_type
  {
    prop_type(){}

    prop_type(const std::size_t ind, const double val)
      : index(ind)
      , value(val)
    {}

    std::size_t index;
    double value;
    bool operator<(const prop_type& in) const {return this->value < in.value;}
  };

public:

  /**
   * Construct the graph out of the two main JSON files containing the data
   * \param graph_file Path to a JSON file containing (subject,predicate,value) triples
   * \param properties_file Path to a JSON file containing a properties object for each subject/node
   */
  graph(const std::string& graph_file, const std::string& properties_file, const bool undirected = false);

  /**
   * Query the graph
   * \param query Currently the exact name of a node
   * \param depth The depth of the search
   */
  std::string query_graph(const std::string& query, const std::size_t depth, const bool by_name) const;

  std::vector<std::size_t> query_graph_parallel(const std::string& query, const std::size_t depth, const std::size_t N, const std::string property, const bool by_name) const;

  std::vector<std::size_t> query_graph_exact(const std::string& query, const std::size_t N, const std::string property, const bool by_name) const;

  std::vector<std::size_t> query_graph_random(const std::string& query, const std::size_t depth, const std::size_t N, const std::string property, const bool by_name) const;

  /**
   * Print out nodes connected to the looked-up one up to a certain depth
   */
  void dump_nodes(const std::string& query, const std::size_t depth, const bool by_name) const;

  /**
   * Find a limited set of nodes based on property comparison (currently assumed to be a double)
   * \return A map of the calculated distance to the respective path leading to this node
   */
  std::map<double,std::vector<size_t> > find_nodes_closest_by_property_comparison(
      const std::string& query,
      const std::size_t depth,
      const bool by_name,
      const std::string & property,
      const std::size_t limit) const;

  /**
   * Create additional edges based on similarities within given properties
   * \param properties List of pairs of properties and threshold value
   */
  void create_property_edges(const std::vector<std::pair<std::string, double> > properties);

  /**
   * Get the node corresponding to a given index
   */
  node_type get(size_t i) const
  {
    return nodes_[i];
  }

  /**
   * Get the number of nodes
   */
  size_t count() const
  {
    return nodes_.size();
  }

  std::size_t find_node_id(const std::string&) const;
  std::size_t find_node_name(const std::string&) const;

  double compute_error(const std::string& query, const std::string property, const bool by_name, const std::vector<std::size_t>& indices) const;
  
private:

  void add_similarity(const std::string property, const double threshold);

  void visit_nodes_bfs(
      const std::size_t root,
      std::function<bool (const std::vector<size_t> &)> f,
      const std::size_t depth = std::numeric_limits<size_t>::max()
  ) const;

  //*******

  bool undirected_;
  std::vector<node_type> nodes_;
  std::unordered_map<std::string, std::size_t> lookup_id_, lookup_name_;
};

#endif
