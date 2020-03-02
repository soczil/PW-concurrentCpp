#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <chrono>
#include <iostream>

void assert_msg(bool condition, std::string const& msg);

void assert_eq_msg(uint64_t val1, uint64_t val2, std::string const& msg);

std::chrono::time_point<std::chrono::high_resolution_clock> getCurrentTime();

double getTimeDifference(
    std::chrono::time_point<std::chrono::high_resolution_clock> const&
        startTime);

template <class F>
void runAndPrintDuration(F&& lambda) {
  auto startTime = getCurrentTime();
  lambda();
  std::cout << getTimeDifference(startTime) << std::endl;
}

#endif  // SRC_UTILS_H_
