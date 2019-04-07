LETTER [a-zA-Z_]
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





[gG][eE][tT]			{
					return get;
				}
[fF][rR][oO][mM]		{
					return from;
				}
[wW][hH][eE][rR][eE]		{
					return where;
				}
([">""<"]["="]?)|("!="|"==")	{
					yylval.str=strdup(yytext);
					return ops;
				}
[aA][nN][dD]			{
					return and;
				}
[oO][rR]			{
					return or;
				}







[uU][pP][dD][aA][tT][eE]	{
					return update;
				}
[iI][nN]			{
					return in;
				}

[sS][eE][tT]			{
					return set;
				}
[tT][oO]			{
					return to;
				}


[dD][eE][lL][eE][tT][eE]	{
					//printf("got delete\n");
					return delete;
				}






{LETTER}({LETTER}|{DIGIT}|"\$")+ {
					//printf("name:%s\n",yytext);
					yylval.str=strdup(yytext);
					return name;
				}
{DIGIT}+			{
					//printf("number:%s\n",yytext);
					yylval.str=strdup(yytext);
					return number;
				}
"\n"				{return 0;}
[ ]				{}

%%
