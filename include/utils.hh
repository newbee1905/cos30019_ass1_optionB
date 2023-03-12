#ifndef UTILS_HH
#define UTILS_HH

#include <array>
#include <string_view>

/// convert the list of __VA_ARGS__ into a string
#define CAT(...) #__VA_ARGS__

/// get the number of element inside __VA_ARGS__
/// at compile time
template <typename... Args>
constexpr std::size_t va_count(Args&&...) {
  return sizeof...(Args);
}

/// split string at compile time
/// return an array of element
template <std::size_t N>
constexpr std::array<std::string_view, N> split(std::string_view str,
                                                char delim = ',') {
  std::array<std::string_view, N> arr{};
  std::size_t start = 0, end = 0;

  for (std::size_t i = 0; i < N && end != std::string_view::npos; ++i) {
    end = str.find_first_of(delim, start);
    arr[i] = str.substr(start, end - start);
    start = end + 1;
  }

  return arr;
}

#endif  // !UTILS_HH
