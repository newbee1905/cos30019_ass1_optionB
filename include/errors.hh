/// This file is for custom error handling functions
/// They will be put inside namespace kd which
/// is a shortcut stands for my nickname

#ifndef ERRORS_HH
#define ERRORS_HH

#include <assert.h>

#include "fmt/core.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace kd {

// TODO: fix error handling on windows
// currently on window it is still counted
// as unhandled exception

/// Create a custom class to handle runtime error
/// that has information about th eline of code that
/// it is throwing from
class runtime_error : public std::runtime_error {
	std::string msg;

public:
	runtime_error(const std::string &arg, const char *file, int line) : std::runtime_error(arg) {
		msg = fmt::format("{}:{}: {}", file, line, arg);
	}
	~runtime_error() throw() {}

	const char *what() const throw();
};
}; // namespace kd

#define throw_line(arg) throw kd::runtime_error(arg, __FILE__, __LINE__)

#define assert_line(cond, arg)                                                                     \
	do {                                                                                             \
		if (!cond)                                                                                     \
			throw_line(arg);                                                                             \
	} while (0)

#endif // !ERRORS_HH
