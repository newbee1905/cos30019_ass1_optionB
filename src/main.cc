#include <iostream>
#include <string_view>

#include "e_methods.hh"

signed main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Not enough arguments. "
                 "Please use: ./search <FILEPATH> <METHOD>\n";
    return 1;
  }

  std::string_view file = argv[1];
  std::string_view method = argv[2];

  std::cout << "You select method " << method << ' ' << GetEnumMethods(method);
}
