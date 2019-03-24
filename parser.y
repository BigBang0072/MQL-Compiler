%{
    	#include <stdio.h>
	#include <string.h>
	extern FILE *yyin;
	int yylex(void);
	void yyerror(const char *str);
	
	//Defining the strings for holding the query data
	#define str_size 100000
	char filename[str_size]="empty";
	char fields[str_size]="";
    
%}

%union{
	int num;
	char *str;
}
%token insert record into comma bopen bclose
%token <str> Name
%type <str> Enteries
%%
	QUERY:		insert record TableEntry into Name	{
								strcpy(filename,$5);
								//printf("**Filenames:%s\n",filename);

								};
	
	TableEntry:	bopen Enteries bclose;
	
	Enteries:	Enteries comma Name			{
								strcat(fields,",");
								strcat(fields,$3);
								//printf("**Enteries:%s\n",fields);
								}
			|
			Name					{
								//printf("**Entry:%s\n",$1);
								strcpy(fields,$1);
								};
%%

int main(){
	while(1){
		printf("\nPlease enter the Query:\n");
		//Starting the parsing of file
		if(!yyparse()){
			printf("fields:%s\n",fields);
			printf("filename:%s\n",filename);
			printf("Success Man\n\n");
		}
		else{
			printf("Faliure Man\n");
		}
	}
}	
