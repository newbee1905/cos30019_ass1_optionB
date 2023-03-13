/// This file is for custom error handling functions
/// They will be put inside namespace kd which
/// is a shortcut stands for my nickname

#ifndef ERRORS_HH
#define ERRORS_HH

#include <assert.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace kd {

/// Create a custom class to handle runtime error
/// that has information about th eline of code that
/// it is throwing from
class runtime_error : public std::runtime_error {
  std::string msg;

 public:
  runtime_error(const std::string &arg, const char *file, int line);
  ~my_exception() throw() {}

  const char *what() const throw();
};
#define throw_line(arg) throw my_exception(arg, __FILE__, __LINE__);
};  // namespace kd

#endif // !ERRORS_HH
