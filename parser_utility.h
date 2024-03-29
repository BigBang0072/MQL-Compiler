#ifndef PARSE_UTILS
#define PARSE_UTILS

/*
Defining the struct for the for the record of EMPLOYEE Table
*/
#define num_emp_field 6
#define int_fsize 11
#define str_fsize 50
int EMP_FSIZE[num_emp_field]={int_fsize,str_fsize,\
                                int_fsize,str_fsize,\
                                int_fsize,int_fsize};
typedef struct EMP_Record{
    //Defining the total size of the record
    unsigned int tsize;

    //Defining the EMPrecord character array
    char *record;
}EMP_Record;
char emp_fname[20]="EMP";

/*
Defining the struct for the record of DEPARTMENT Table
*/
#define num_dept_field 3
int DEPT_FSIZE[num_dept_field]={int_fsize,str_fsize,str_fsize};
typedef struct DEPT_Record{
    //Defining the total size of record
    unsigned int tsize;

    //Defining the DEPT_Record character array
    char *record;
}DEPT_Record;
char dept_fname[20]="DEPT";

//Initialization functions
EMP_Record * get_emp_object();
DEPT_Record* get_dept_object();
//Getting the field number from the name
int get_field_num(char table_name,char *field_name);


//Defining the struct to hold the condition
struct scond{
    //will be either [and,or]  [>,<,=,>=,<=,==,!=]
    char *op_name;

    //These will be used when we will use the cond as leaf
    char *field_name;
    char *num_name;

    //These will be active in place when this struct is not leaf
    struct scond* left;
    struct scond* right;
};
//Get query handling
void evaluate_record(int rec_num,char rec_type,int record_len,char *record,\
                        char fields[],struct scond *root);
char* get_field_from_record(char *field_name,char rec_type,char* record);
int apply_comparison_op(char rec_type,char *field_name,char *field_cont,\
                            char *op_name,char *num_name);
int traverse_cond_tree(char rec_type,int record_len,char *record,\
                        struct scond *root);
void get_from_table(char fields[],char filename[],struct scond *root);

#endif
