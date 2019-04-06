#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(){
    printf("val:%d\n",strcmp("abc","abc"));
    // if(1){
    //     int j=2;
    // }
    // printf("j: %d\n",j);
    //printf("val: %ld\n",strtol(" 123,abc",NULL,10));

    void *ptr;
    int temp=10;
    ptr=&temp;
    printf("%d\n",*(int *)ptr);
    return 0;
}
