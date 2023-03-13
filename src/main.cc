#include <stdio.h>

#include <iostream>
#include <string_view>

#include "e_methods.hh"
#include "fmt/core.h"

signed main(int argc, char **argv) {
  if (argc < 3) {
    fmt::print(stderr,
               "Not enough arguments. "
               "Please use: ./search <FILEPATH> <METHOD>\n");
    return 1;
  }

  std::string_view file = argv[1];
  std::string_view method = argv[2];

  fmt::print("You select method {}", GetEnumMethods(method));
}
