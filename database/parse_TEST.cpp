
#include "parse.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    EdgeFile ef("../edges_test.txt");

    ef.load([](const std::string & s, const std::string & p, const std::string & v) {
        std::cout << s << " - " << p << " - " << v << std::endl;
        return true;
    });
    return 0;
}
