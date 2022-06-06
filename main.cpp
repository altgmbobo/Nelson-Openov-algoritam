#include <iostream>
#include <memory>
#include <utility>
#include <map>
#include <iterator>
#include <algorithm>
#include "term.h"

using namespace std;

using Eq = vector<pair<TermPtr,TermPtr>>;
using Domain = vector<TermPtr>;
Domain d;
Eq eq;
map<TermPtr, TermPtr> findT;
map<TermPtr, vector<TermPtr>> useT;

bool equal(TermPtr &t1, TermPtr &t2); //
bool use(TermPtr t, TermPtr in); //
bool cong(TermPtr s, TermPtr t); //
void unionT(TermPtr t1, TermPtr t2); //
void merge(TermPtr t1, TermPtr t2); //
void print_solution(); //
void print_term(TermPtr it); //
void print_class(vector<TermPtr> cl);
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
    cout << "solution:";
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
        print_class(it.second);
    }
}

void start(){
    for(auto term : d){
        useT.insert(pair<TermPtr,vector<TermPtr>>(term, vector<TermPtr>()));
        findT[term] = term;
        for(const auto user : d)
            if(use(term, user))
                useT[term].push_back(user);
    }

    for(auto& it : eq)
        merge(it.first, it.second);

}

void merge(TermPtr t1, TermPtr t2){
    vector<TermPtr> v1;
    vector<TermPtr> v2;
    for(const auto& it : findT){
        if(equal((TermPtr&) it.second,(TermPtr&) findT[t1])){
            for(const auto term : d)
                if(use(it.first, term))
                    v1.push_back(term);
        }
    }

    for(const auto it : findT){
        if(equal((TermPtr&) it.second,(TermPtr&) findT[t2])){
            for(const auto term : d)
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
                for(int i = 0; i < s->function.args.size(); i++){
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
    for(const auto t : d){
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
                for(int i = 0; i < t1->function.args.size(); i++)
                    ind = ind && equal(t1->function.args[i], t2->function.args[i]);
                return ind;
            }
    }
    return false;
}
void print_term(TermPtr term) {
    switch(term->type) {
    case Term::Variable:
        cout << term->variable;
        break;
    case Term::Function:
        cout << term->function.name;
        if(!term->function.args.empty()) {
            cout << "(";
            print_term(term->function.args[0]);
            for(auto it = term->function.args.begin() + 1; it != term->function.args.end(); it++) {
                cout << ", ";
                print_term(*it);
            }
            cout << ")";
        }
    }
}



int main(){
    for(char c = 'x'; c < 'z' + 1; c++){
        auto s = string(1, c);
        d.push_back(VariableT(s));
    }
    d.push_back(FunctionT("f", {d[0], d[1]}));
    d.push_back(FunctionT("f", {d[2], d[1]}));
    d.push_back(VariableT("a"));
    eq.push_back(pair<TermPtr, TermPtr>(d[0], d[1]));
    eq.push_back(pair<TermPtr, TermPtr>(d[1], d[2]));
//    for(auto it : d){
//        print_term(it);
//        cout << endl;
//    }
    start();
    print_solution();
//    pair<int, char> p;
//    p.first = 100;
//    p.second = 'a';
//    cout << p.first << ' ' << p.second;
//    pair<int,int> p = std::make_pair<int,int>(1,1);
//    cout << p.first << " " << p.second;

}


//inicijalizacija use mape i find mape
//pokretanje merge-a
