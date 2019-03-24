LETTER [a-zA-Z]
DIGIT [0-9]

%{
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
%}

%%
[iI][nN][sS][eE][rR][tT]	{
					//printf("insert:%s\n",yytext);
					return insert;
				}
[rR][eE][cC][oO][rR][dD]	{	
					//printf("record:%s\n",yytext);
					return record;
				}
"("				{
					//printf("bopen:%s\n",yytext);
					return bopen;
				}
")"				{
					//printf("bclose:%s\n",yytext);
					return bclose;
				}
","				{
					//printf("comma:%s\n",yytext);
					return comma;
				}
[iI][nN][tT][oO]		{
					//printf("into:%s\n",yytext);
					return into;
				}
{LETTER}({LETTER}|{DIGIT})+ 	{
					//printf("Name:%s\n",yytext);
					yylval.str=strdup(yytext);
					return Name;
				}
"\n"				{return 0;}
[ ]				{}

%%
