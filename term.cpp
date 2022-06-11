#include "term.h"

#include <iostream>


TermPtr VariableT(std::string s){
    return std::make_shared<Term>(VariableTerm{s});
}
TermPtr FunctionT(std:: string symbol, const std::vector<TermPtr>& args){
    return std::make_shared<Term>(FunctionTerm{symbol, args});
}

bool equal(TermPtr &t1, TermPtr &t2)
{
    if(t1->type != t2->type)
        return false;
    switch(t1->type){
        case Term::Type::Variable:
            if(!t1->variable.compare(t2->variable))
                return true;
            else
                return false;
        case Term::Type::Function:
            if(t1->function.name.compare(t2->function.name))
                return false;
            else if(t1->function.args.size() != t2->function.args.size())
                return false;
            else{
                bool ind = true;
                for(unsigned i = 0; i < t1->function.args.size(); i++)
                    ind = ind && equal(t1->function.args[i], t2->function.args[i]);
                return ind;
            }
    }
    return false;
}


void print_term(TermPtr term) {
    switch(term->type) {
    case Term::Variable:
        std::cout << term->variable;
        break;
    case Term::Function:
        std::cout << term->function.name;
        if(!term->function.args.empty()) {
            std::cout << "(";
            print_term(term->function.args[0]);
            for(auto it = term->function.args.begin() + 1; it != term->function.args.end(); it++) {
                std::cout << ", ";
                print_term(*it);
            }
            std::cout << ")";
        }
    }
}
