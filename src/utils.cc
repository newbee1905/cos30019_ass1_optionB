/// This file is for some ultility functions
/// They will be put inside namespace kd which
/// is a shortcut stands for my nickname

#include "utils.hh"

std::string_view kd::tolower_string(std::string_view str) {
	std::string lowerstr(str.size(), '\0');
	std::transform(str.begin(), str.end(), lowerstr.begin(),
	               [](unsigned char c) { return std::tolower(c); });
	return std::string_view{lowerstr};
}
