/// This file is for custom error handling functions
/// They will be put inside namespace kd which
/// is a shortcut stands for my nickname

#include "errors.hh"

kd::runtime_error(const std::string &arg, const char *file, int line)
	: std::runtime_error(arg) {
	std::ostringstream o;
	o << file << ":" << line << ": " << arg;
	msg = o.str();
}

const char *kd::runtime_error::what() const throw() {
	return msg.c_str(); 
}
