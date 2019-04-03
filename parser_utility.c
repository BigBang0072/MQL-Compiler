#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser_utility.h"

//Getting the field number from the name
int get_field_num(char table_name,char *field_name){
    /*
    This function will give the field number in the database given
    the name of the field.
    */
    //If the field is from Employee
    if(table_name=='e'){
        if(!strcmp(field_name,"eid")){
            return 0;
        }
        else if(!strcmp(field_name,"ename")){
            return 1;
        }
        else if(!strcmp(field_name,"eage")){
            return 2;
        }
        else if(!strcmp(field_name,"eaddress")){
            return 3;
        }
        else if(!strcmp(field_name,"salary")){
            return 4;
        }
        else if(!strcmp(field_name,"deptno")){
            return 5;
        }
        else{
            printf("Wrong field name received\n");
            exit(0);
        }
    }
    else{
        if(strcmp(field_name,"dnum")){
            return 0;
        }
        else if(strcmp(field_name,"dname")){
            return 1;
        }
        else if(strcmp(field_name,"dlocation")){
            return 2;
        }
        else{
            printf("Wrong field name received\n");
            exit(0);
        }
    }
    printf("Wrong table name specified\n");
    return -1;
}

//Function to initialize the EMP record struct
EMP_Record* get_emp_object(){
    /*
    This function will give default EMP record set with appropriate
    field and a character array of approriate size of the heap
    */
    //Getting the record from heap
    EMP_Record* Erecord=(EMP_Record*)malloc(sizeof(EMP_Record));

    //Assigning approriate size to each field size
    for(int i=0;i<num_emp_field;i++){
        Erecord->tsize+=EMP_FSIZE[i];
    }

    //Creating an empty array from the heap
    char *record=(char*)malloc(sizeof(char)*Erecord->tsize);
    Erecord->record=record;

    return Erecord;
}

//Function to initialize the Department record struct
DEPT_Record* get_dept_object(){
    /*
    This function will initialize the department object appropriately
    and return the pointer to the Dept-Record
    */
    //Getting the record from heap
    DEPT_Record* Drecord=(DEPT_Record*)malloc(sizeof(DEPT_Record));

    //Assigning the appropriate size to each field
    for(int i=0;i<num_dept_field;i++){
        Drecord->tsize+=DEPT_FSIZE[i];
    }

    //Initializing the character array from heap
    char *record=(char *)malloc(sizeof(char)*Drecord->tsize);
    Drecord->record=record;

    return Drecord;
}

//
