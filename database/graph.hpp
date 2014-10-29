#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "parse.hpp"
#include "utility.hpp"

#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <set>
#include <unordered_map>

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

  //*******

  std::string id_;
  PropertyContainer properties_;
  std::string name_;
  std::vector<std::pair<std::size_t,std::string> > neighbours_;
};

class graph
{

public:

  /**
   * Construct the graph out of the two main JSON files containing the data
   * \param graph_file Path to a JSON file containing (subject,predicate,value) triples
   * \param properties_file Path to a JSON file containing a properties object for each subject/node
   */
  graph(const std::string& graph_file, const std::string& properties_file);

  /**
   * Query the graph
   * \param query Currently the exact name of a node
   * \param depth The depth of the search
   */
  std::string query_graph(const std::string& query, const std::size_t depth) const;

  /**
   * Insert a new node
   */
  void insert_node(const node_type&);

private:

  void build_sub_graph_depth(const std::size_t, const std::size_t, std::set<std::size_t>&, std::string&) const;

  void build_sub_graph_breadth(const std::vector<std::size_t>&, const std::size_t, std::set<std::size_t>&, std::string&) const;

  std::size_t find_node(const std::string&) const;

  //*******

  std::vector<node_type> nodes_;
};

#endif
