#ifndef PARSE_UTILS
#define PARSE_UTILS
/*
Defining the struct for the for the record of EMPLOYEE Table
*/
#define num_emp_field 6
int EMP_FSIZE[num_emp_field]={11,50,11,50,11,11};
typedef struct EMP_Record{
    //Defining the total size of the record
    unsigned int tsize;

    //Defining the EMPrecord character array
    char *record;
}EMP_Record;

/*
Defining the struct for the record of DEPARTMENT Table
*/
#define num_dept_field 3
int DEPT_FSIZE[num_dept_field]={11,50,50};
typedef struct DEPT_Record{
    //Defining the total size of record
    unsigned int tsize;

    //Defining the DEPT_Record character array
    char *record;
}DEPT_Record;

//Initialization functions
EMP_Record * get_emp_object();
DEPT_Record* get_dept_object();
//Getting the field number from the name
int get_field_num(char table_name,char *field_name);

#endif
