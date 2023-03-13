#ifndef ENUM_STR_HH
#define ENUM_STR_HH

#include <array>
#include <fstream>
#include <string_view>

#include "utils.hh"

/// Auto generate an array of string for enum value
///
/// FIX: Doesn't work with enum that has custom value
#define ENUM_STR(ENUM_NAME, ...)                                       \
  enum ENUM_NAME { __VA_ARGS__ };                                      \
  constexpr const size_t va_len = kd::va_count(__VA_ARGS__);           \
  constexpr std::array<std::string_view, va_len> __tag_##ENUM_NAME__ = \
      kd::split<va_len>(CAT(__VA_ARGS__));                             \
  constexpr std::string_view GetString##ENUM_NAME(ENUM_NAME index) {   \
    return __tag_##ENUM_NAME__[index];                                 \
  }                                                                    \
  std::ostream& operator<<(std::ostream& o, const ENUM_NAME& e) {      \
    o << GetString##ENUM_NAME(e);                                      \
    return o;                                                          \
  }

/// Auto generate a constexpr map of enum value
/// as in string_view and as enum value
///
/// FIX: Doesn't work with enum that has custom value
#define ENUM_MAP(ENUM_NAME, ...)                                               \
  enum ENUM_NAME { __VA_ARGS__ };                                              \
  constexpr const size_t va_len = kd::va_count(__VA_ARGS__);                   \
  constexpr std::array<kd::pair<std::string_view, ENUM_NAME>, va_len>          \
      __tag_map_##ENUM_NAME__ =                                                \
          kd::split_enum<va_len, ENUM_NAME>(CAT(__VA_ARGS__));                 \
  static constexpr auto __map_##ENUM_NAME__ =                                  \
      kd::map<std::string_view, ENUM_NAME, va_len>{{__tag_map_##ENUM_NAME__}}; \
  constexpr std::string_view GetString##ENUM_NAME(ENUM_NAME index) {           \
    return __map_##ENUM_NAME__.data[index].fst;                                \
  }                                                                            \
  constexpr ENUM_NAME GetEnum##ENUM_NAME(const std::string_view k) {           \
    return __map_##ENUM_NAME__[k];                                             \
  }                                                                            \
  std::ostream& operator<<(std::ostream& o, const ENUM_NAME& e) {              \
    o << GetString##ENUM_NAME(e);                                              \
    return o;                                                                  \
  }

#endif  // !ENUM_STR_HH
