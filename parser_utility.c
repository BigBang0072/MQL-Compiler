#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser_utility.h"

/////////////////////////////////////////////////////////////////////
/*                  General Utility Functions                      */
/////////////////////////////////////////////////////////////////////
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
        if(!strcmp(field_name,"dnum")){
            return 0;
        }
        else if(!strcmp(field_name,"dname")){
            return 1;
        }
        else if(!strcmp(field_name,"dlocation")){
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
char* get_record_array(char type, int *record_len){
    /*
    This function will give default record set with appropriate
    field and a character array of approriate size of the heap.
    */
    //Assigning approriate size to each field size
    int arr_size=0;
    if(type=='e'){
        for(int i=0;i<num_emp_field;i++){
            arr_size+=EMP_FSIZE[i];
        }
    }
    else if(type=='d'){
        //Assigning the appropriate size to each field
        for(int i=0;i<num_dept_field;i++){
            arr_size+=DEPT_FSIZE[i];
        }
    }
    //Adding extra length for the "\0"
    arr_size++;
    //Assigning the length to the record len
    *record_len=arr_size;

    //Creating an empty array from the heap
    char *record=(char*)malloc(sizeof(char)*(arr_size));

    return record;
}
//File Handle opening function
FILE* get_file_handle(char filename[],char mode[]){
    /*
    This function will take the file name and return the file handle
    in appropriate mode.
    */
    //First appening the .txt into the file name
    strcat(filename,".txt");

    //Opening the file
    FILE* fp=fopen(filename,mode);
    if(fp==NULL){
        printf("Error opening the file\n");
        exit(0);
    }
    printf("File:%s \topened in mode:%s\n",filename,mode);
    return fp;
}
//Test if the string is vallid integer or not based on table desc
int get_integer(char buffer[]){
    int num=strtol(buffer,NULL,10);
    return num;
}

/////////////////////////////////////////////////////////////////////
/*                  Handling the insert query                      */
/////////////////////////////////////////////////////////////////////
//inserting the particular field to record
void insert_field_into_record(int field_num,char temp_buff[],\
                                char *record,int FSIZE[]){
    /*
    This function will appropriately fill the field into respective
    palce in record according to the field num provided.
    */
    //Calcualting the position in the record
    int insert_loc=0;
    for(int i=0;i<field_num;i++){
        insert_loc+=FSIZE[i];
    }

    //Now we can copy the buffer from the insert loc
    int buff_size=strlen(temp_buff);
    for(int i=0;i<buff_size;i++){
        record[insert_loc+i]=temp_buff[i];
    }
    //Now putting space from last posiiton to the field size
    for(int i=buff_size;i<FSIZE[field_num];i++){
        record[insert_loc+i]=' ';
    }
}
//Main function to control all the insertion
void insert_into_table(char tentry[],char filename[]){
    /*
    This function will create a new entry into the record with the
    given filename.
    */
    //Creating the employee record object
    char *record;
    char rec_type;
    int record_len;
    printf("\nInserting record to the table\n");
    if(!strcmp(filename,emp_fname)){
        record=get_record_array('e',&record_len);
        rec_type='e';
    }
    else if(!strcmp(filename,dept_fname)){
        record=get_record_array('d',&record_len);
        rec_type='d';
    }
    else{
        printf("Retry: Wrong file name given.\n");
        return;
    }
    printf("Initialized record array of type: %c and len:%d\n",\
                                rec_type,record_len);

    //Getting the file pointer
    FILE* fp=get_file_handle(filename,"a+");

    int entry_len=strlen(tentry);
    if(entry_len>=record_len){
        printf("Table entry is larger than specified size\n");
        exit(0);
    }

    //Now iterating over the table entry
    int field_num=0;
    //Initializing the temp bffer to hold each fields
    char temp_buff[str_fsize+1];
    int buff_idx=0;
    //Iterating ove the table entry
    for(int i=0;i<entry_len;i++){
        printf("i:%d ent_len:%d buff: %s\n",i,entry_len,temp_buff);
        if(tentry[i]!=','){
            //Copying the entry into buffer
            temp_buff[buff_idx]=tentry[i];
            buff_idx++;         //will point where to enter
        }
        if(tentry[i]==',' || (i==entry_len-1)){
            //Appending /0 to end of buffer
            temp_buff[buff_idx]='\0';

            if(rec_type=='e' && EMP_FSIZE[field_num]==int_fsize){
                //If this is integer field then we will have to verify
                int num=get_integer(temp_buff);
                if(num==0){
                    printf("Expected Integer for field:%d\n",field_num);
                    printf("Retry: Not inserting the entry\n");
                    return;
                }
                //If the dtype is correct, then insert the field into rec
                insert_field_into_record(field_num,temp_buff,record,\
                                                        EMP_FSIZE);
            }
            else if(rec_type=='d' && DEPT_FSIZE[field_num]==int_fsize){
                //If this is integer field then we will have to verify
                int num=get_integer(temp_buff);
                if(num==0){
                    printf("Expected Integer for field:%d\n",field_num);
                    printf("Retry: Not inserting the entry\n");
                    return;
                }
                //If the dtype is correct, then insert the field into rec
                insert_field_into_record(field_num,temp_buff,record,\
                                                        DEPT_FSIZE);
            }
            else if(rec_type=='e'){
                //This means that the field is string field
                insert_field_into_record(field_num,temp_buff,record,\
                                            EMP_FSIZE);
            }
            else if(rec_type=='d'){
                insert_field_into_record(field_num,temp_buff,record,\
                                            DEPT_FSIZE);
            }
            //Incrementing the field number and clearning buffer
            field_num++;
            //temp_buff="";
            buff_idx=0;
        }
    }
    //Finally after all the insertion we will append \0 to end of string
    record[record_len-1]='\0';
    printf("Record Created:\n%s\n",record);
    printf("length of record: %ld\n",strlen(record));

    //Saving the record to the file
    //printf("File pointer loc: %ld\n",ftell(fp));
    fputs(record,fp);
    fputs("\n",fp);
    fclose(fp);
    //Deallocating the record created from heap;
    free(record);
}
