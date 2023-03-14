/// This file is for custom error handling functions
/// They will be put inside namespace kd which
/// is a shortcut stands for my nickname

#include "errors.hh"

#include "fmt/core.h"

const char *kd::runtime_error::what() const throw() {
  return msg.c_str();
}
