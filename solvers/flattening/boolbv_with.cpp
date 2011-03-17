/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <std_types.h>
#include <arith_tools.h>

#include "boolbv.h"

/*******************************************************************\

Function: boolbvt::convert_with

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void boolbvt::convert_with(const exprt &expr, bvt &bv)
{
  if(expr.operands().size()<3)
    throw "with takes at least three operands";

  if((expr.operands().size()%2)!=1)
    throw "with takes an odd number of operands";

  convert_bv(expr.op0(), bv);

  unsigned width;
  if(boolbv_get_width(expr.type(), width))
    return conversion_failed(expr, bv);

  if(bv.size()!=width)
    throw "unexpected operand 0 width";

  bvt prev_bv;
  prev_bv.resize(width);

  const exprt::operandst &ops=expr.operands();

  for(unsigned op_no=1; op_no<ops.size(); op_no+=2)
  {
    bv.swap(prev_bv);

    convert_with(expr.op0().type(),
                 ops[op_no],
                 ops[op_no+1],
                 prev_bv,
                 bv);
  }
}

/*******************************************************************\

Function: boolbvt::convert_with

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void boolbvt::convert_with(
  const typet &type,
  const exprt &op1,
  const exprt &op2,
  const bvt &prev_bv,
  bvt &next_bv)
{
  // we only do that on arrays, bitvectors, structs, and unions

  next_bv.resize(prev_bv.size());

  if(type.id()=="array")
    return convert_with_array(to_array_type(type), op1, op2, prev_bv, next_bv);
  else if(type.id()=="bv" ||
          type.id()=="unsignedbv" ||
          type.id()=="signedbv")
    return convert_with_bv(type, op1, op2, prev_bv, next_bv);
  else if(type.id()=="struct")
    return convert_with_struct(type, op1, op2, prev_bv, next_bv);
  else if(type.id()=="union")
    return convert_with_union(type, op1, op2, prev_bv, next_bv);

  throw "unexpected with type";
}

/*******************************************************************\

Function: boolbvt::convert_with_array

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void boolbvt::convert_with_array(
  const array_typet &type,
  const exprt &op1,
  const exprt &op2,
  const bvt &prev_bv,
  bvt &next_bv)
{
  if(is_unbounded_array(type))
  {
    // can't do this    
    throw "convert_with_array called for unbounded array";
  }

  const exprt &array_size=type.size();

  mp_integer size;

  if(to_integer(array_size, size))
    throw "convert_with_array expects constant array size";
    
  bvt op2_bv;
  convert_bv(op2, op2_bv);

  if(size*op2_bv.size()!=prev_bv.size())
    throw "convert_with_array: unexpected operand 2 width";

  mp_integer op1_value;
  if(!to_integer(op1, op1_value))
  {
    next_bv=prev_bv;

    if(op1_value<size)
    {
      unsigned offset=integer2long(op1_value*op2_bv.size());

      for(unsigned j=0; j<op2_bv.size(); j++)
        next_bv[offset+j]=op2_bv[j];
    }

    return;
  }

  typet counter_type=op1.type();

  for(mp_integer i=0; i<size; i=i+1)
  {
    exprt counter=from_integer(i, counter_type);

    exprt eq_expr("=", typet("bool"));
    eq_expr.operands().reserve(2);
    eq_expr.copy_to_operands(op1);
    eq_expr.copy_to_operands(counter);

    literalt eq_lit=convert(eq_expr);

    unsigned offset=integer2long(i*op2_bv.size());

    for(unsigned j=0; j<op2_bv.size(); j++)
      next_bv[offset+j]=
        prop.lselect(eq_lit, op2_bv[j], prev_bv[offset+j]);
  }
}

/*******************************************************************\

Function: boolbvt::convert_with_bv

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void boolbvt::convert_with_bv(
  const typet &type,
  const exprt &op1,
  const exprt &op2,
  const bvt &prev_bv,
  bvt &next_bv)
{
  literalt l=convert(op2);

  mp_integer op1_value;
  if(!to_integer(op1, op1_value))
  {
    next_bv=prev_bv;

    if(op1_value<next_bv.size())
      next_bv[integer2long(op1_value)]=l;

    return;
  }

  typet counter_type=op1.type();

  for(unsigned i=0; i<prev_bv.size(); i++)
  {
    exprt counter=from_integer(i, counter_type);

    exprt eq_expr("=", typet("bool"));
    eq_expr.operands().reserve(2);
    eq_expr.copy_to_operands(op1);
    eq_expr.copy_to_operands(counter);

    literalt eq_lit=convert(eq_expr);

    next_bv[i]=prop.lselect(eq_lit, l, prev_bv[i]);
  }
}

/*******************************************************************\

Function: boolbvt::convert_with

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void boolbvt::convert_with_struct(
  const typet &type,
  const exprt &op1,
  const exprt &op2,
  const bvt &prev_bv,
  bvt &next_bv)
{
  next_bv=prev_bv;

  bvt op2_bv;
  convert_bv(op2, op2_bv);

  const irep_idt &component_name=op1.get("component_name");
  const irept &components=type.find("components");

  unsigned offset=0;

  forall_irep(it, components.get_sub())
  {
    unsigned sub_width;

    const typet &subtype=static_cast<const typet &>
      (it->find("type"));

    if(boolbv_get_width(subtype, sub_width))
      sub_width=0;

    if(it->get("name")==component_name)
    {
      if(subtype!=op2.type())
        throw "with/struct: component `"+id2string(component_name)+
          "' type does not match: "+
          subtype.to_string()+" vs. "+
          op2.type().to_string();

      if(sub_width!=op2_bv.size())
        throw "convert_with_struct: unexpected operand op2 width";

      for(unsigned i=0; i<sub_width; i++)
        next_bv[offset+i]=op2_bv[i];
        
      break; // done
    }

    offset+=sub_width;
  }
}

/*******************************************************************\

Function: boolbvt::convert_with_union

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

void boolbvt::convert_with_union(
  const typet &type,
  const exprt &op1,
  const exprt &op2,
  const bvt &prev_bv,
  bvt &next_bv)
{
  next_bv=prev_bv;

  bvt op2_bv;
  convert_bv(op2, op2_bv);

  if(next_bv.size()<op2_bv.size())
    throw "convert_with_union: unexpected operand op2 width";

  for(unsigned i=0; i<op2_bv.size(); i++)
    next_bv[i]=op2_bv[i];
    
  const struct_typet &s_type=to_struct_type(type);

  const irep_idt &component_name=op1.get("component_name");

  if(!s_type.has_component(component_name))
    throw "with/union: component not found";

  unsigned number=s_type.component_number(component_name);

  unsigned component_bits=
    integer2long(address_bits(s_type.components().size()));

  unsigned offset=next_bv.size()-component_bits;

  for(unsigned i=0; i<component_bits; i++)
    next_bv[offset+i]=const_literal((number>>i)&1);
}

