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

    //*******

    std::string id_;
    PropertyContainer properties_;
    std::string name_;
    std::vector<std::pair<std::size_t,std::string> > neighbours_;
  };

  struct prop_type
  {
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
  graph(const std::string& graph_file, const std::string& properties_file);

  /**
   * Query the graph
   * \param query Currently the exact name of a node
   * \param depth The depth of the search
   */
  std::string query_graph(const std::string& query, const std::size_t depth, const bool by_name) const;

private:

  void build_sub_graph(const std::vector<std::size_t>&, const std::size_t, std::set<std::size_t>&, std::string&) const;

  void add_similarity(const std::string property, const double threshold);

  std::size_t find_node(const std::string&) const;
  std::size_t find_node_id(const std::string&) const;
  std::size_t find_node_name(const std::string&) const;

  //*******

  std::vector<node_type> nodes_;
  std::unordered_map<std::string, std::size_t> lookup_id_, lookup_name_;
};

#endif
