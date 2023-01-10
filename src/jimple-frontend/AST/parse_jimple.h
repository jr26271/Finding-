#pragma once

#include <string_view>
#include <sstream>
#include <cstdlib>

class parse_jimple {
public:
  // TODO: This should be in path
  inline static std::string_view jimple2json = "jimple2json";
  static void convert_jimple_file(const std::string_view file) {
    std::ostringstream  oss;
    oss << jimple2json << " " << file;
    std::system(oss.str().c_str());
  }
};