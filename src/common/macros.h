#ifndef SRC_COMMON_MACROS_H
#define SRC_COMMON_MACROS_H

#include <cstring>
#include <iostream>

inline auto ASSERT(bool cond, std::string_view msg) noexcept {
  if (!cond) [[unlikely]] {
    std::cerr << "ASSERT : " << msg << std::endl;

    exit(EXIT_FAILURE);
  }
}

inline auto FATAL(std::string_view msg) noexcept {
  std::cerr << "FATAL : " << msg << std::endl;

  exit(EXIT_FAILURE);
}

#endif // SRC_COMMON_MACROS_H