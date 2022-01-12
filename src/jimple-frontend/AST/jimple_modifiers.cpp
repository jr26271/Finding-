//
// Created by rafaelsamenezes on 21/09/2021.
//

#include <jimple-frontend/AST/jimple_modifiers.h>
void jimple_modifiers::from_json(const json &j)
{
  auto p = j.get<std::vector<std::string>>();
  for(auto &x : p)
  {
    this->m.push_back(from_string(x));
  }
}

jimple_modifiers::modifier
jimple_modifiers::from_string(const std::string &name) const
{
  return from_map.at(name);
}
std::string
jimple_modifiers::to_string(const jimple_modifiers::modifier &ft) const
{
  return to_map.at(ft);
}
std::string jimple_modifiers::to_string() const
{
  std::ostringstream oss;

  oss << "Modifiers: ||";
  for(auto &x : this->m)
  {
    oss << " " << to_string(x) << " |";
  }
  oss << "|";

  return oss.str();
}