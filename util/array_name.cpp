/*******************************************************************\

Module: Misc Utilities

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include "array_name.h"

/*******************************************************************\

Function: goto_checkt::array_name

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

std::string array_name(
  const namespacet &ns,
  const exprt &expr)
{
  if(expr.id()=="index")
  {
    if(expr.operands().size()!=2)
      throw "index takes two operands";

    return array_name(ns, expr.op0())+"[]";
  }
  else if(expr.id()=="symbol")
  {
    const symbolt &symbol=ns.lookup(expr);
    return "array `"+id2string(symbol.base_name)+"'";
  }
  else if(expr.id()=="string-constant")
  {
    return "string";
  }

  return "array";
}

