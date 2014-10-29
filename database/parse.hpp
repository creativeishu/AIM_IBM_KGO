#ifndef PARSE_HPP
#define PARSE_HPP

#include <string>
#include <array>
#include <fstream>
#include <functional>
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

#endif
