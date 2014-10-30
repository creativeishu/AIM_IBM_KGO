#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <chrono>

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

class ClockMeasure
{
private:
  std::chrono::time_point<std::chrono::system_clock> start_, end_;
  std::chrono::duration<double> elapsed_seconds_;
public:
  void tick()
  {
    start_ = std::chrono::system_clock::now();
  }
  double tock()
  {
    end_ = std::chrono::system_clock::now();
    elapsed_seconds_ = end_ - start_;
    return elapsed_seconds_.count();
  }
};
#endif
