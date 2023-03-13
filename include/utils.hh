/// This file is for some ultility functions
/// They will be put inside namespace kd which
/// is a shortcut stands for my nickname

#ifndef UTILS_HH
#define UTILS_HH

#include <algorithm>
#include <array>
#include <exception>
#include <iterator>
#include <string_view>

/// convert the list of __VA_ARGS__ into a string
#define CAT(...) #__VA_ARGS__

namespace kd {

/// No constructor custom struct
/// Should have the best constructor possible
/// from the compiler side
///
/// The kd::pair should be similar to std::pair
/// after optimisation in Clang and GCC
///
/// However, for msvc, this version has less
/// lines of asm code
template <typename T1, typename T2>
struct pair {
  T1 fst;
  T2 sec;
};

/// get the number of element inside __VA_ARGS__
/// at compile time
template <typename... Args>
constexpr std::size_t va_count(Args &&...) {
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
    start = end + 2;  // remove the space after the ','
  }

  return arr;
}

/// split string at compile time
/// return an array of enum with
/// its string and index value
template <std::size_t N, typename E>
constexpr std::array<kd::pair<std::string_view, E>, N> split_enum(
    std::string_view str, char delim = ',') {
  std::array<kd::pair<std::string_view, E>, N> arr{};
  std::size_t start = 0, end = 0;

  for (std::size_t i = 0; i < N && end != std::string_view::npos; ++i) {
    end = str.find_first_of(delim, start);
    arr[i] = kd::pair<std::string_view, E>{
        str.substr(start, end - start),
        E(i),
    };
    start = end + 2;  // remove the space after the ','
  }

  return arr;
}

/// Map built at compile time
/// Linear search instead of log N
/// Will be optimised by compiler and more suitable
/// for small map
template <typename key, typename val, std::size_t size>
struct map {
  std::array<kd::pair<key, val>, size> data;

  [[nodiscard]] constexpr val at(const key &k) const {
    const auto itr = std::find_if(std::begin(data), std::end(data),
                                  [&k](const auto &v) { return v.fst == k; });
    if (itr != end(data))
      return itr->sec;
    else
      throw std::range_error("Not Found");
  }

  [[nodiscard]] constexpr val operator[](const key &k) const { return at(k); }
};

};  // namespace kd

#endif  // !UTILS_HH
