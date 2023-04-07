#include "agent.hh"
#include "e_action.hh"
#include "e_methods.hh"
#include "fmt/ostream.h"

const kd::Cell &kd::Agent::pos() { return m_pos; }
void kd::Agent::set_pos(const kd::Cell &pos) { m_pos = pos; }
void kd::Agent::set_pos(const kd::Cell &&pos) { m_pos = std::move(pos); }
