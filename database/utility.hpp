#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

template <class T0, class T1>
std::ostream& operator<<(std::ostream& os, const std::pair<T0,T1>& x)
{
  os << "(" << x.first << ' ' << x.second << ")";
  return os;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& x)
{
  std::copy(x.begin(),x.end(),std::ostream_iterator<T>(os," "));
  return os;
}

#endif
