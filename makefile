all:
	clang++ -std=c++11 -o main token.cpp lexer.cpp parser.cpp main.cpp

