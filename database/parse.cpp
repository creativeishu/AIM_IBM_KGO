
#include "parse.hpp"

#include <fstream>
#include <stdexcept>

#include "thirdparty/rapidjson/filereadstream.h"
#include "thirdparty/rapidjson/document.h"

EdgeFile::EdgeFile(const std::string & filename) :
  fh_(nullptr),
  is_(nullptr)
{
  fh_ = fopen(filename.c_str(), "r");

  if (!fh_)
    throw std::runtime_error(std::string("Could not open file: ") + filename);

  is_ = new rapidjson::FileReadStream(fh_, &readBuffer_.front(), readBuffer_.size());
}

void EdgeFile::load(std::function<bool (const std::string &, const std::string &, const std::string &)> f)
{
  rapidjson::Document d;

  if (d.ParseStream(*is_).HasParseError())
    throw std::runtime_error("invalid json");

  if (!d.IsArray())
    throw std::runtime_error("Root element is not an array");

  for (auto i = d.Begin(); i != d.End(); ++i)
  {
    if (!i->IsArray())
      throw std::runtime_error("Subelement is not an array");

    if (! f((*i)[0].GetString(), (*i)[1].GetString(), (*i)[2].GetString()) )
      return;
  }
}

EdgeFile::~EdgeFile()
{
  if (fh_)
    fclose(fh_);
  if (is_)
    delete is_;
}

PropertiesFile::PropertiesFile(const std::string & filename) :
  fh_(nullptr),
  is_(nullptr)
{
  fh_ = fopen(filename.c_str(), "r");

  if (!fh_)
    throw std::runtime_error(std::string("Could not open file: ") + filename);

  is_ = new rapidjson::FileReadStream(fh_, &readBuffer_.front(), readBuffer_.size());
}

void PropertiesFile::load(std::function<bool (const std::string &, PropertyContainer &&)> f)
{
  rapidjson::Document d;

  if (d.ParseStream(*is_).HasParseError())
    throw std::runtime_error("invalid json");

  if (!d.IsObject())
    throw std::runtime_error("Root element is not an object");

  for (auto n_i = d.MemberBegin(); n_i != d.MemberEnd(); ++n_i)
  {
    PropertyContainer p;

    if (!n_i->value.IsObject())
      throw std::runtime_error(std::string("Properties of node '")
          + n_i->name.GetString()
          + "' are not not an object");

    for (auto p_i = n_i->value.MemberBegin(); p_i != n_i->value.MemberEnd(); ++p_i)
    {
      // TODO: the idea is to store the types natively, but this either needs
      // a "any"-type or a custom type with the visitor pattern

      if (! (p_i->value.IsString() || p_i->value.IsDouble()))
        throw std::runtime_error(
            std::string("Value of property '")
            + p_i->name.GetString()
            + "' is not a string or a double");

      std::string value(p_i->value.IsString() ? p_i->value.GetString() : std::to_string(p_i->value.GetDouble()));
      p.insert(std::make_pair<std::string,std::string>(p_i->name.GetString(), std::move(value)));
    }


    if (! f(n_i->name.GetString(), std::move(p)) )
      return;
  }

}

PropertiesFile::~PropertiesFile()
{
  if (fh_)
    fclose(fh_);
  if (is_)
    delete is_;
}
