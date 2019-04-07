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
//Get a buffer record of appropriate type
char* get_buffer_record(char filename[],char *rec_type,int *record_len){
    /*
    This function will initialize a buffer acc to table
    */
    char *record;
    if(!strcmp(filename,emp_fname)){
        record=get_record_array('e',record_len);
        *rec_type='e';
    }
    else if(!strcmp(filename,dept_fname)){
        record=get_record_array('d',record_len);
        *rec_type='d';
    }
    else{
        printf("Retry: Wrong file name given.\n");
        return NULL;
    }
    printf("Initialized record array of type: %c and len:%d\n",\
                                *rec_type,*record_len);
    return record;
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
    record=get_buffer_record(filename,&rec_type,&record_len);
    if(record==NULL){
        return;
    }

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

/////////////////////////////////////////////////////////////////////
/*                 Handling the get query                          */
/////////////////////////////////////////////////////////////////////
void evaluate_record(int rec_num,char rec_type,int record_len,char *record,\
                        char fields[],struct scond *root){
    /*
    This function will evaluate the record and print them if the condition
    holds true for this particular record.
    */
    //Testing if the condition holds true for the fields
    int eval=traverse_cond_tree(rec_type,record_len,record,root);

    //If the condition holds then we will reterive the element and print
    if(eval){
        printf("Record:%d staisfies the condition\n",rec_num);
        int fields_len=strlen(fields);
        char field_buff[fields_len];
        int buff_idx=0;
        for(int i=0;i<fields_len;i++){
            if(fields[i]!=','){
                field_buff[buff_idx]=fields[i];
                buff_idx++;
            }
            if(fields[i]==',' || (i==fields_len-1)){
                //Appending end character to the string
                field_buff[buff_idx]='\0';
                //Now retreiving the field content
                //printf("field name:%s\n",field_buff);
                char *field_cont=get_field_from_record(field_buff,rec_type,\
                                                        record);
                //Printing the resutls
                printf("%s, ",field_cont);

                //Resetting the buffer index
                buff_idx=0;
            }
        }
        printf("\n");
    }
    return;

}
//Given a field name get the content of the record
char* get_field_from_record(char *field_name,char rec_type,char* record){
    /*
    This function will retreive the value in the field and return it in
    string buffer.
    */
    //Getting the field index
    int fidx=get_field_num(rec_type,field_name);
    //Getting the length of field
    int flen;
    if(rec_type=='e'){
        flen=EMP_FSIZE[fidx];
    }
    else if(rec_type=='d'){
        flen=DEPT_FSIZE[fidx];
    }
    else{
        printf("Wrong record type mentioned. Retry\n");
        exit(0);
    }
    //Calcualting the offset to start the string copying
    int offset=0;
    for(int i=0;i<fidx;i++){
        if(rec_type=='e'){
            offset+=EMP_FSIZE[i];
        }
        else{
            offset+=DEPT_FSIZE[i];
        }
    }

    //Now creating a string in heap to copy the string
    char *field_cont=(char*)malloc(sizeof(flen+1));
    field_cont[flen]='\0';
    //Copying the content of record to the string
    for(int i=0;i<flen;i++){
        if(record[offset+i]!=' '){
            field_cont[i]=record[offset+i];
        }
        else{
            field_cont[i]='\0';
            break;
        }
    }
    return field_cont;
}
//Applying the comparison
int apply_comparison_op(char rec_type,char *field_name,char *field_cont,\
                            char *op_name,char *num_name){
    /*
    This function will aply the comparison operator to the field value and
    the value mentioned in the consition.
    */
    //Retreiving the field type
    int fidx=get_field_num(rec_type,field_name);
    //Getting the field type
    char ftype;
    if(rec_type=='e' && EMP_FSIZE[fidx]==int_fsize){
        ftype='i';
    }
    else if(rec_type=='d' && DEPT_FSIZE[fidx]==int_fsize){
        ftype='i';
    }
    else{
        ftype='s';
    }

    //Now comparing based on the operator and field type
    if(!strcmp(op_name,">") && ftype=='i'){
        if(atoi(field_cont)>atoi(num_name)){
            return 1;
        }
        return 0;
    }
    else if(!strcmp(op_name,"==")){
        if(ftype=='i' && (atoi(field_cont)==atoi(num_name))){
            return 1;
        }
        else if(ftype=='s' && (!strcmp(field_cont,num_name))){
            return 1;
        }
        return 0;
    }
    else if(!strcmp(op_name,"!=")){
        if(ftype=='i' && (atoi(field_cont)!=atoi(num_name))){
            return 1;
        }
        else if(ftype=='s' && strcmp(field_cont,num_name)){
            return 1;
        }
        return 0;
    }
    else if(!strcmp(op_name,"<") && ftype=='i'){
        if(atoi(field_cont)<atoi(num_name)){
            return 1;
        }
        return 0;
    }
    else if(!strcmp(op_name,">=") && ftype=='i'){
        if(atoi(field_cont)>=atoi(num_name)){
            return 1;
        }
        return 0;
    }
    else if(!strcmp(op_name,"<=") && ftype=='i'){
        if(atoi(field_cont)<=atoi(num_name)){
            return 1;
        }
        return 0;
    }
    //Other wise the comparison is not valid
    printf("Comparison not valid. Recheck the condition.\n");
    return 0;
}
//Traversing the tree to evaluate condition on record.
int traverse_cond_tree(char rec_type,int record_len,char *record,\
                        struct scond *root){
    /*
    We will traverse the tree and evaluate the condition returning
    whether this record satisfy the condition.
    */
    //Handling the base case (both will be NULL other wise both wont be)
    if(root->left==NULL && root->right==NULL){
        //ie when we are at the leaf node. only the comparaters are there
        //Retreiving the field operator and number
        char *field_name=root->field_name;
        char *op_name=root->op_name;
        char *num_name=root->num_name;

        //Extracting the field value of form the record
        char *field_cont=get_field_from_record(field_name,rec_type,record);
        //Now applying the comparison operator
        return apply_comparison_op(rec_type,field_name,field_cont,\
                                    op_name,num_name);
    }

    //Now we are free of the comparison. We just have to handle the && or ||
    //Solving left and right subproblem
    int left_eval=traverse_cond_tree(rec_type,record_len,record,root->left);
    int right_eval=traverse_cond_tree(rec_type,record_len,record,root->right);

    if(!strcmp(root->op_name,"and")){
        return (left_eval && right_eval);
    }
    else if(!strcmp(root->op_name,"or")){
        return (left_eval || right_eval);
    }
    else{
        printf("Wrong operator in the non-leaf node.Check code\n");
        exit(0);
    }
    //Will never come
    return 0;
}
//Free up the pointers used in condition tree
void free_up_cond_tree(struct scond *root){
    /*
    This fucntion will free up the memory allocated in the heap
    for getting rid of dangling pointer.
    */
    //Freeing up the leaves node
    if(root->left==NULL && root->right==NULL){
        free(root->field_name);
        free(root->op_name);
        free(root->num_name);
        return;
    }
    //Recusively freeing the left and right child
    free_up_cond_tree(root->left);
    free_up_cond_tree(root->right);
    //Freeing up itself now once its decendent are destroyed
    free(root->op_name);
    free(root);
    return;
}
//main handler for getting the records from the table.
void get_from_table(char fields[],char filename[],struct scond *root){
    /*
    This function will read the table line by line and then evaluate
    the condition stored in the condition tree and then retreive
    those fields from the row and print them.
    */
    //Getting the temporary buffer based on the filename
    char *record;
    char rec_type;
    int record_len;
    printf("\nGetting records from the table\n");
    record=get_buffer_record(filename,&rec_type,&record_len);
    if(record==NULL){
        return;
    }
    //Getting the filehandle
    FILE* fp=get_file_handle(filename,"r");

    //Getting the records one by one to check which one stays below
    int rec_num=0;
    printf("\nPlease Wait:Searching the database\n");
    while(fgets(record,record_len+1,fp)!=NULL){
        //printf("%s",record);
        evaluate_record(rec_num,rec_type,record_len,record,fields,root);
        rec_num++;
    }

    //Freeing up the full condition tree
    free_up_cond_tree(root);
}

////////////////////////////////////////////////////////////////////
/*                 Handling the update query                      */
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
/*                 Handling the update query                      */
////////////////////////////////////////////////////////////////////
void deleteLine(FILE *srcFile, FILE *tempFile,int delline[10000])
{
	char buffer[1000];
	int count = 1;
	int i = 0;
	int line = delline[0];
	while ((fgets(buffer, BUFFER_SIZE, srcFile)) != NULL)
	{
		/* If current line is not the line user wanted to remove */
		if (line != count)
		{
			fputs(buffer, tempFile);
		}
		else
		{
			i++;
			line = delline[i];
		}
		count++;
	}
}
void delete_from_table(char filename[], struct scond *root)
{
	/*
	This function will read the table line by line and then evaluate
	the condition stored in the condition tree and then retreive
	those fields from the row and print them.
	*/
	//Getting the temporary buffer based on the filename
	printf("In delete record\n");
	char *record;
	char rec_type;
	int record_len;
	printf("\nGetting records from the table\n");
	record = get_buffer_record(filename, &rec_type, &record_len);
	if (record == NULL) {
		return;
	}
	//Getting the filehandle
	FILE* fp = get_file_handle(filename, "r");
	FILE* tmp = fopen("tmpfile.tmp", "w");
	//Getting the records one by one to check which one stays below
	int rec_num = 0;
	int delline[10000];
	int i = 0;
	printf("\nPlease Wait:Searching the database\n");
	while (fgets(record, record_len + 1, fp) != NULL) 
	{
		rec_num++;
		if (traverse_cond_tree(rec_type, record_len, record, root))
		{
			delline[i] = rec_num;
			i++;
		}
	}
	delline[i] = 10000;
	rewind(fp);
	deleteLine(fp,tmp,delline);
	fclose(fp);
	fclose(tmp);
	remove(filename);
	rename("tmpfile.tmp", path);
}