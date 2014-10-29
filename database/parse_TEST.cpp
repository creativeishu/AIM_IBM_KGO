
#include "parse.hpp"

#include <iostream>

int main(int, char **)
{
    EdgeFile ef("../edges_test.txt");

    ef.load([](const std::string & s, const std::string & p, const std::string & v) {
        std::cout << s << " - " << p << " - " << v << std::endl;
        return true;
    });

    PropertiesFile pf("../props_test.txt");
    pf.load([](const std::string & s, PropertiesFile::PropertyContainer && props) {
        std::cout << std::endl << s << std::endl;
        for (const auto & p: props)
            std::cout << "\t" << p.first << ": " << p.second << std::endl;
        return true;
    });
    return 0;
}
