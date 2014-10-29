#ifndef PARSE_HPP
#define PARSE_HPP

#include <string>
#include <array>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <stdexcept>
#include <error.h>

#include "thirdparty/rapidjson/filereadstream.h"
#include "thirdparty/rapidjson/document.h"

const size_t MAX_READ_BUFFER = 65536;

class EdgeFile
{
public:
    EdgeFile(const std::string & filename) :
        fh_(nullptr),
        is_(nullptr)
    {
        fh_ = fopen(filename.c_str(), "r");

        if (!fh_)
            throw std::runtime_error(std::string("Could not open file: ") + strerror(errno));

        is_ = new rapidjson::FileReadStream(fh_, &readBuffer_.front(), readBuffer_.size());
    }

    void load(std::function<bool (const std::string &, const std::string &, const std::string &)> f)
    {
        rapidjson::Document d;

        if (d.ParseStream(*is_).HasParseError())
            return;
        
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

    ~EdgeFile()
    {
        if (fh_)
            fclose(fh_);
        if (is_)
            delete is_;
    }

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

    PropertiesFile(const std::string & filename) :
        fh_(nullptr),
        is_(nullptr)
    {
        fh_ = fopen(filename.c_str(), "r");

        if (!fh_)
            throw std::runtime_error(std::string("Could not open file: ") + strerror(errno));

        is_ = new rapidjson::FileReadStream(fh_, &readBuffer_.front(), readBuffer_.size());
    }

    void load(std::function<bool (const std::string &, PropertyContainer &&)> f)
    {
        rapidjson::Document d;

        if (d.ParseStream(*is_).HasParseError())
            return;

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
                if (!p_i->value.IsString())
                    throw std::runtime_error(
                            std::string("Value of property '")
                            + p_i->name.GetString()
                            + "' is not a string");

                p.insert(std::make_pair<std::string,std::string>(p_i->name.GetString(), p_i->value.GetString()));
            }


            if (! f(n_i->name.GetString(), std::move(p)) )
                return;
        }

    }

    ~PropertiesFile()
    {
        if (fh_)
            fclose(fh_);
        if (is_)
            delete is_;
    }

    // we have state which whould require deep copy/movement
    PropertiesFile(const PropertiesFile &) = delete;
    PropertiesFile& operator=(const PropertiesFile &) = delete;

private:
    std::array<char, MAX_READ_BUFFER> readBuffer_;
    FILE * fh_;
    rapidjson::FileReadStream * is_;
};

#endif
