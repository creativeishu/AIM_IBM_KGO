#ifndef PARSE_HPP
#define PARSE_HPP

#include <string>
#include <array>
#include <functional>
#include <unordered_map>

const size_t MAX_READ_BUFFER = 65536;

// Forward declaration for foreign namespace
namespace rapidjson {
    class FileReadStream;
}

class EdgeFile
{
public:
  EdgeFile(const std::string & filename);
  void load(std::function<bool (const std::string &, const std::string &, const std::string &)> f);
  ~EdgeFile();

  // we have state which whould require deep copy/movement
  EdgeFile(const EdgeFile &) = delete;
  EdgeFile& operator=(const EdgeFile &) = delete;

private:
  std::array<char, MAX_READ_BUFFER> readBuffer_;
  FILE * fh_;
  rapidjson::FileReadStream * is_;
};

class PropertiesFile
{
public:
  typedef std::unordered_map<std::string, std::string> PropertyContainer;

  PropertiesFile(const std::string & filename);
  void load(std::function<bool (const std::string &, PropertyContainer &&)> f);
  ~PropertiesFile();

  // we have state which whould require deep copy/movement
  PropertiesFile(const PropertiesFile &) = delete;
  PropertiesFile& operator=(const PropertiesFile &) = delete;

private:
  std::array<char, MAX_READ_BUFFER> readBuffer_;
  FILE * fh_;
  rapidjson::FileReadStream * is_;
};

#endif
