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
	struct scond *root;

%}

%union{
	int num;
	char *str;
	struct scond *sc;
}


%token insert record into comma bopen bclose
%token <str> name
%token <str> number
%type <str> Enteries

%token get from where and or
%token <str> ops
%type <sc> Cond
%type <sc> Cond1
%type <sc> Cond2

%%
	QUERY:		INSERT_QUERY				{//Inserting into the table by creating record
								insert_into_table(tentry,filename);}
			|
			GET_QUERY 				{};










	INSERT_QUERY:	insert record TableEntry into name	{
								strcpy(filename,$5);
								//printf("**Filenames:%s\n",filename);
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








      GET_QUERY:	get Fields from name where Cond1	{
								strcpy(filename,$4);
								//Getting the condition statement tree's root
								root=($6);

								//Printing and sanity test
								printf("filename: %s\n",filename);
								printf("op:%s field:%s num:%s\n",root->op_name,root->field_name,root->num_name);
								printf("c1op:%s c2op:%s \n",root->left->op_name,root->right->op_name);
								};

      Fields:		Fields comma name			{
								strcat(fields,",");
								strcat(fields,$3);
								}
			|
			name					{strcat(fields,$1);};

      Cond1:		Cond1 or Cond2				{//Now its time to make new node
								//Creating new string for op
								char *op_name=(char*)malloc(sizeof(char)*5);
								strcpy(op_name,"or");

								//making the condition struct
								$$=(struct scond*)malloc(sizeof(struct scond));

								$<sc->op_name>$=op_name;

								//Filling the left and right
								$<sc->left>$=($1);
								$<sc->right>$=($3);

								//Filling NULL to leaf attributes
								$<sc->field_name>$=NULL;
								$<sc->num_name>$=NULL;
								}
			|
			Cond2					{
								//making the condition struct
								$$=(struct scond*)malloc(sizeof(struct scond));

								//Directly passing this child node above ie. copying
								$<sc->op_name>$=$<sc->op_name>1;

								$<sc->field_name>$=$<sc->field_name>1;
								$<sc->num_name>$=$<sc->num_name>1;

								$<sc->left>$=$<sc->left>1;
								$<sc->right>$=$<sc->right>1;};

      Cond2:		Cond2 and Cond				{
								//Now its time to make new node
								//Creating new string for op
								char *op_name=(char*)malloc(sizeof(char)*5);
								strcpy(op_name,"and");

								//making the condition struct
								$$=(struct scond*)malloc(sizeof(struct scond));

								$<sc->op_name>$=op_name;

								//Filling the left and right
								$<sc->left>$=($1);
								$<sc->right>$=($3);
								
								//Priting the condition
								//printf("Cond2: name:%s op:%s number:%s\n",$1->field_name,$1->op_name,$1->num_name);
								//printf("Cond2: name:%s op:%s number:%s\n",$3->field_name,$3->op_name,$3->num_name);

								//Filling NULL to leaf attributes
								$<sc->field_name>$=NULL;
								$<sc->num_name>$=NULL;
								};
			|
			Cond					{
								//making the condition struct
								$$=(struct scond*)malloc(sizeof(struct scond));

								//Directly passing this child node above ie. copying
								$<sc->op_name>$=$<sc->op_name>1;

								$<sc->field_name>$=$<sc->field_name>1;
								$<sc->num_name>$=$<sc->num_name>1;

								$<sc->left>$=$<sc->left>1;
								$<sc->right>$=$<sc->right>1;
								};

      Cond:		name ops number				{
								//Getting the number operator and field as a string
								//Deallocate appropriately later.
								char *nstr=(char*)malloc(sizeof(char)*strlen($1));
								char *ostr=(char*)malloc(sizeof(char)*strlen($2));
								char *nustr=(char*)malloc(sizeof(char)*strlen($3));

								//Copying the value in string
								strcpy(nstr,$1);
								strcpy(ostr,$2);
								strcpy(nustr,$3);

								//Priting the condition
								printf("Cond: name:%s op:%s number:%s\n",nstr,ostr,nustr);

								//making the condition struct
								$$=(struct scond*)malloc(sizeof(struct scond));
								$<sc->field_name>$=nstr;
								$<sc->op_name>$=ostr;
								$<sc->num_name>$=nustr;
								//Indicating that there is no furthur child
								$<sc->left>$=NULL;
								$<sc->right>$=NULL;
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
