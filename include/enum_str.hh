#ifndef ENUM_STR_HH
#define ENUM_STR_HH

#include <array>
#include <string_view>

#include "utils.hh"

/// Auto generate an array of string for enum value
///
/// FIX: Doesn't work with enum that has custom value
#define ENUM_STR(ENUM_NAME, ...)                                            \
  enum ENUM_NAME { __VA_ARGS__ };                                           \
  constexpr std::array<std::string_view, va_count(__VA_ARGS__)>             \
      __tag_##ENUM_NAME__ = split<va_count(__VA_ARGS__)>(CAT(__VA_ARGS__)); \
  constexpr std::string_view GetString##ENUM_NAME(ENUM_NAME index) {        \
    return __tag_##ENUM_NAME__[index];                                      \
  }

#endif  // !ENUM_STR_HH
