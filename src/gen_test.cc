#include "agent.hh"
#include "e_action.hh"
#include "e_block.hh"
#include "e_methods.hh"
#include "fmt/core.h"
#include "grid.hh"
#include "isearch.hh"
#include "search.hh"
#include <algorithm>
#include <string>
#include <tuple>

signed main(int argc, char **argv) {
	if (argc < 5) {
		fmt::print(stderr, "Not enough arguments. "
		                   "Please use: gen_test <HEIGHT> <WIDTH> <START> <END>\n");
		return 1;
	}

	int h = std::stoi(argv[1]);
	int w = std::stoi(argv[2]);
	int s = std::stoi(argv[3]);
	int e = std::stoi(argv[4]);
	kd::Grid g(h, w, 1);
	g.gen(s, e);
	g.write_gen(s, e);
}
