/*******************************************************************\

Module: SMT-LIB Builtin Logics, Finalizer for the QF_IDL logic 

Author: CM Wintersteiger

\*******************************************************************/

#ifndef SMT_FINALIZER_QF_IDL_H_
#define SMT_FINALIZER_QF_IDL_H_

#include "builtin_theories.h"

#include "smt_finalizer_generic.h"

class smt_typecheck_expr_QF_IDLt : public smt_typecheck_expr_generict
{
protected:  
  virtual typet search_fun(const exprt&, const typet&);
  virtual typet search_sort(const typet&);
  
public:
  smt_typecheck_expr_QF_IDLt(  message_streamt &eh, const exprt& b, 
                                const symbolst &t) : 
    smt_typecheck_expr_generict(eh, b, t)
    {};  
};

class smt_finalizer_QF_IDLt : public smt_finalizer_generict
{
  private:
    void check_double_sorts(const exprt&, const symbolst&, message_streamt&);
    void check_double_functions(const exprt&, const symbolst&, message_streamt&);    
    void check_double_predicates(
      const exprt&, const symbolst&, message_streamt&); 
       
  public:
    smt_finalizer_QF_IDLt(
      contextt &c,
      message_handlert &mh) :
        smt_finalizer_generict(c, mh)
      {};
    
    virtual ~smt_finalizer_QF_IDLt( void ) {};    
      
    virtual std::string logic( void ) const
      { return std::string("QF_IDL"); }
    
    virtual bool finalize( const exprt& );
};

#endif /*SMT_FINALIZER_QF_IDL_H_*/
