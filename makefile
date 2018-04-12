all:
	clang++ -std=c++11 -o main token.cpp lexer.cpp parser.cpp types.cpp expressions.cpp declarations.cpp statements.cpp main.cpp
