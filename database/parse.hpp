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

/**
 * Load an edges JSON file
 */
class EdgeFile
{
public:
  EdgeFile(const std::string & filename);
  ~EdgeFile();

  /**
   * Parse the file opened during construction
   * \param f The function to call for every found (subject,predicate,value) triple,
   *          can return false to terminate parsing, true to continue.
   */
  void load(std::function<bool (const std::string &, const std::string &, const std::string &)> f);

  /**
   * Delete copy constructor due to file handles present
   */
  EdgeFile(const EdgeFile &) = delete;

  /**
   * Delete assignment constructor due to file handles present
   */
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
  ~PropertiesFile();

  /**
   * Parse the file opened during construction
   * \param f The function to call for every found node entry containing a properties object,
   *          can return false to terminate parsing, true to continue.
   */
  void load(std::function<bool (const std::string &, PropertyContainer &&)> f);

  /**
   * Delete copy constructor due to file handles present
   */
  PropertiesFile(const PropertiesFile &) = delete;

  /**
   * Delete assignment constructor due to file handles present
   */
  PropertiesFile& operator=(const PropertiesFile &) = delete;

private:
  std::array<char, MAX_READ_BUFFER> readBuffer_;
  FILE * fh_;
  rapidjson::FileReadStream * is_;
};

#endif
