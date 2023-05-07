#ifndef __MAIN_GUI__
#define __MAIN_GUI__

#include "SDL.h"
#include "agent.hh"
#include "fmt/chrono.h"
#include "fmt/core.h"
#include "grid.hh"
#include "isearch.hh"

int gui(kd::Grid &grid, kd::Agent &a, std::shared_ptr<kd::ISearch> search);

#endif // __MAIN_GUI__
