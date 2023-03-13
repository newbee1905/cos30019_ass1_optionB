#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string_view>

#include "e_methods.hh"
#include "fmt/core.h"
#include "scn/detail/file.h"
#include "scn/scn.h"

signed main(int argc, char** argv) {
  if (argc < 3) {
    fmt::print(stderr,
               "Not enough arguments. "
               "Please use: ./search <FILEPATH> <METHOD>\n");
    return 1;
  }

  char* inp_file_name = argv[1];
  std::string_view method = argv[2];

  fmt::print("You select method {}\n", GetEnumMethods(method));
  auto f = fopen(inp_file_name, "r");
  scn::file inp_file{f};
  std::string line;
  for (auto result = scn::getline(inp_file, line);
       result.error() != scn::error::end_of_range;
       result = scn::getline(result.range(), line)) {
    fmt::println("{}", line);
  }

  fclose(f);
}
