/// This file is for some ultility functions
/// They will be put inside namespace kd which
/// is a shortcut stands for my nickname

#include "utils.hh"

FILE* kd::cfile_impl(buffer_t* const fb) {
  return (static_cast<io_buffer_t* const>(fb))->file();  // type std::__c_file
}

FILE* kd::cfile(std::ofstream const& ofs) {
  return cfile_impl(ofs.rdbuf());
}
FILE* kd::cfile(std::ifstream const& ifs) {
  return cfile_impl(ifs.rdbuf());
}
