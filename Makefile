main: main.cpp term.h term.cpp parser.tab.o lex.yy.o
	g++ -Wall -o $@ $^

lex.yy.o: lex.yy.c parser.tab.hpp
	g++ -Wall -c -o $@ $<

parser.tab.o: parser.tab.cpp parser.tab.hpp term.h lex.yy.o
	g++ -Wall -c -o $@ $<

parser.tab.cpp parser.tab.hpp: parser.ypp term.h term.cpp
	bison -d -v parser.ypp

lex.yy.c: lexer.lex
	flex $<


.PHONY: clean

clean:
	rm -f *.output *.o lex.yy.* parser.tab.* main *~
