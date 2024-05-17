#pragma once

#include <stdio.h>
#include <vector>

namespace stmbed::utility {
template <class... Args> inline std::vector<char> format(const char *fmt, Args... args) {
  size_t len = snprintf(nullptr, 0, fmt, args...) + 1;
  std::vector<char> buf(len);
  snprintf(buf.data(), len, fmt, args...);
  return buf;
}
} // namespace stmbed::utility
