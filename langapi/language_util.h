/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@cs.cmu.edu

\*******************************************************************/

#ifndef CPROVER_LANGUAGE_UTIL_H
#define CPROVER_LANGUAGE_UTIL_H

#include <namespace.h>
#include <language.h>

std::string from_expr(
  const namespacet &ns,
  const irep_idt &identifier,
  const exprt &expr);

std::string from_expr(const exprt &expr);

std::string from_type(
  const namespacet &ns,
  const irep_idt &identifier,
  const typet &type);

std::string from_type(const typet &type);

#endif
