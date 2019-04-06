%{
    	#include <stdio.h>
	#include <string.h>
	#include "parser_utility.c"

	extern FILE *yyin;
	int yylex(void);
	void yyerror(const char *str);
	
	//Defining the strings for holding the query data
	#define str_size 10000
	char filename[str_size]="";
	char fields[str_size]="";
	char tentry[str_size]="";
    
%}

%union{
	int num;
	char *str;
}
%token insert record into comma bopen bclose
%token <str> name
%token <str> number

%type <str> Enteries
%%
	QUERY:		insert record TableEntry into name	{
								strcpy(filename,$5);
								//printf("**Filenames:%s\n",filename);
								//Inserting into the table by creating record
								insert_into_table(tentry,filename);

								};
	
	TableEntry:	bopen Enteries bclose;
	
	Enteries:	Enteries comma name			{
								strcat(tentry,",");
								strcat(tentry,$3);
								//printf("**Enteries:%s\n",fields);
								}
			|
			Enteries comma number			{
								strcat(tentry,",");
								strcat(tentry,$3);
								}
			|
			name					{
								//printf("**Entry:%s\n",$1);
								strcat(tentry,$1);
								}
			|
			number					{
								//printf("**Entry:%s\n",$1);
								strcat(tentry,$1);
								};
%%

int main(){
	while(1){
		printf("\nPlease enter the Query:\n");
		//Starting the parsing of file
		if(!yyparse()){
			//printf("fields:%s\n",tentry);
			//printf("filename:%s\n",filename);
			//printf("Testing the header: %d\n",get_field_num('e',"ename"));
			//printf("TEsting header: %d\n",num_emp_field);

			//Freeing up the char array for next query
			tentry[0]=0;
			fields[0]=0;
			filename[0]=0;
			printf("Success Man\n\n");
		}
		else{
			printf("Faliure Man\n");
		}
	}
}	
