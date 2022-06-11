%option noyywrap
%{
    #include <iostream>
    #include <vector>
    #include <string>
    #include "term.h"
    #include "parser.tab.hpp"
%}


%%
D {return dom_token;}
E {return eq_token;}
[a-z][0-9]*  { yylval.str = new std::string(yytext); return Var;}
[A-Z][0-9]*  { yylval.str = new std::string(yytext); return Fun;}
, return *yytext;
[\=\(\){};\n] { return *yytext;}
[\t ] {}
. {
    std::cerr << "Pogresan karakter: " << *yytext << std::endl;
}

%%
