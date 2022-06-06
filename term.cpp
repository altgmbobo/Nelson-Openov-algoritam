#include "term.h"

#include <iostream>


TermPtr VariableT(std::string s){
    return std::make_shared<Term>(VariableTerm{s});
}
TermPtr FunctionT(std:: string symbol, const std::vector<TermPtr>& args){
    return std::make_shared<Term>(FunctionTerm{symbol, args});
}



