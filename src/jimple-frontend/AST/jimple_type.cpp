#include <jimple-frontend/AST/jimple_type.h>

void jimple_type::from_json(const json &j)
{
  j.at("identifier").get_to(name);
  j.at("dimensions").get_to(dimensions);

  bt = from_map.count(name) != 0 ? from_map[name] : BASE_TYPES::OTHER;
}

typet jimple_type::get_base_type(const contextt &ctx) const
{
  // The size of types are taken from https://kotlinlang.org/spec/pdf/kotlin-spec.pdf
  switch(bt)
  {
  case BASE_TYPES::BYTE:
    return signedbv_typet(8);

  case BASE_TYPES::SHORT:
    return signedbv_typet(16);

  case BASE_TYPES::INT:
    return signedbv_typet(32);

  case BASE_TYPES::LONG:
    return signedbv_typet(64);

  case BASE_TYPES::BOOLEAN:
    return bool_type();

  case BASE_TYPES::_VOID:
    return empty_typet();

  default:
    auto symbol = ctx.find_symbol("tag-" + name);
    if(symbol == nullptr)
      throw "Type not found: " + name;
    return pointer_typet(symbol->type);
  }
}

typet jimple_type::to_typet(const contextt &ctx) const
{
  if(is_array())
    return get_arr_type(ctx);
  return get_base_type(ctx);
}

std::string jimple_type::to_string() const
{
  std::ostringstream oss;
  oss << "Type: " << name << " [" << dimensions << "]";
  return oss.str();
}
