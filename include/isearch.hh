#ifndef I_SEARCH_HH
#define I_SEARCH_HH

#include "e_action.hh"
#include "e_methods.hh"
namespace kd {
class ISearch {
public:
	virtual int step()                        = 0;
	virtual int run()                         = 0;
	virtual const int &nnodes()               = 0;
	virtual int reached_goal()                = 0;
	virtual const std::vector<Action> &path() = 0;
	virtual void trace_path()                 = 0;
	virtual void print_path()                 = 0;
};
} // namespace kd

#endif // !I_SEARCH_HH
