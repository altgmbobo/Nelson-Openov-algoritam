#ifndef TERM_H
#define TERM_H

#include <algorithm>
#include <memory>
#include <string>
#include <vector>


struct Term;
using TermPtr = std::shared_ptr<Term>;
using Eq = std::vector<std::pair<TermPtr,TermPtr>>;
using Domain = std::vector<TermPtr>;

using VariableTerm = std::string;
struct FunctionTerm {
    std::string name;
    std::vector<TermPtr> args;
};

struct Term {
  enum Type {
      Variable,
      Function
  }  type;

  union {
  VariableTerm variable;
  FunctionTerm function;
  };

  Term(VariableTerm v): type(Variable), variable(v) {}
  Term(FunctionTerm f): type(Function), function(f) {}
  ~Term(){
      switch(type){
          case Variable:
              variable.~basic_string();
              break;
          case Function:
              function.~FunctionTerm();
              break;

      }

  }
};

TermPtr VariableT(std::string s);
TermPtr FunctionT(std:: string symbol, const std::vector<TermPtr>& args);
bool equal(TermPtr &t1, TermPtr &t2);
void print_term(TermPtr term);



#endif
