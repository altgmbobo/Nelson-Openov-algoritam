#include <utility>
#include <map>
#include <algorithm>
#include <iostream>
#include "term.h"
#include "parser.tab.hpp"
using namespace std;

extern int yyparse();
extern Domain d;
extern Eq eq;
map<TermPtr, TermPtr> findT;
map<TermPtr, vector<TermPtr>> useT;

bool equal(TermPtr &t1, TermPtr &t2); //
bool use(TermPtr t, TermPtr in); //
bool cong(TermPtr s, TermPtr t); //
void unionT(TermPtr t1, TermPtr t2); //
void merge(TermPtr t1, TermPtr t2); //
void print_solution(); //
void print_class(vector<TermPtr> cl); //
void start(); //

void print_class(vector<TermPtr> cl){
    cout << "{";
    print_term(cl[0]);
        for(auto it = cl.begin()+1; it<cl.end(); it++){
            cout << ",";
            print_term(*it);
        }
    cout << "}";
}


void print_solution(){
    cout << "solution:" << endl;
    cout << "{";
    map<TermPtr, vector<TermPtr>> solution;
    for(auto p : findT){
        solution[p.second].push_back(p.first);
    }
    print_class(solution.begin()->second);
    for(auto s = solution.begin(); s != solution.end(); s++){
        if(s == solution.begin())
            continue;
        cout << ", ";
        print_class(s->second);
    }
    cout << "}" << endl;
    cout << "find:" << endl;
    for(auto& p : findT){
        print_term(p.first);
        cout << " -> ";
        print_term(p.second);
        cout << endl;
    }
    cout << "user:" << endl;
    for(auto& it : useT){
        print_term(it.first);
        cout << " -> ";
        if(it.second.empty()){
            cout << endl;
            continue;
        }
        print_class(it.second);
        cout << endl;
    }
}

void start(){
    for(auto term : d){
        useT.insert(pair<TermPtr,vector<TermPtr>>(term, vector<TermPtr>()));
        findT[term] = term;
        for(auto user : d)
            if(use(term, user)){
                useT[term].push_back(user);
            }
    }

    for(auto& it : eq)
        merge(it.first, it.second);

}

void merge(TermPtr t1, TermPtr t2){
    vector<TermPtr> v1;
    vector<TermPtr> v2;
    for(const auto& it : findT){
        if(equal((TermPtr&) it.second,(TermPtr&) findT[t1])){
            for(const auto& term : d)
                if(use(it.first, term))
                    v1.push_back(term);
        }
    }

    for(const auto& it : findT){
        if(equal((TermPtr&) it.second,(TermPtr&) findT[t2])){
            for(const auto& term : d)
                if(use(it.first, term))
                    v2.push_back(term);
        }
    }
    unionT(t1,t2);
    for(const auto& term1 : v1)
        for(const auto& term2 : v2)
            if(!equal(findT[term1], findT[term2]) && cong(term1, term2))
                merge(term1, term2);
}

bool cong(TermPtr s, TermPtr t){
    if(s->type != t->type)
        return false;
    switch(s->type){
        case Term::Type::Variable:
            return false;
        case Term::Type::Function:
            if(s->function.name.compare(t->function.name))
                return false;
            else if(s->function.args.size() != t->function.args.size())
                return false;
            else{
                bool ind = true;
                for(unsigned i = 0; i < s->function.args.size(); i++){
                    auto it1 = findT.find(s->function.args[i]);
                    auto it2 = findT.find(t->function.args[i]);
                    ind = ind && equal(it1->second, it2->second);
                }
                return ind;
            }

    }
    return false;
}

void unionT(TermPtr t1, TermPtr t2){
    for(const auto& t : d){
        if(equal(findT[t], findT[t2]))
            findT[t] = findT[t1];
    }
    findT[t2] = findT[t1];
}

bool use(TermPtr t, TermPtr in)
{
    switch(in->type){
        case Term::Type::Variable:
            return false;
        case Term::Type::Function:
            for(auto& it : in->function.args){
                if(equal(t, it))
                    return true;
            }
    }
    return false;

}

int main(){
    yyparse();
    start();
    print_solution();
}


