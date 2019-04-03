yacc -d parser.y
lex lexer.l
gcc -o parser_utility.o -c parser_utility.c
gcc y.tab.c lex.yy.c -ly -ll
./a.out
