yacc -d parser.y
lex lexer.l
gcc y.tab.c lex.yy.c -ly -ll
./a.out

