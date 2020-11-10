#include "caller.h" 
int main(int argc, char *argv[]){ 
     int size=20;
     int task_number=0;
     struct prinfo* buffer=(struct prinfo*) malloc(size * sizeof(struct prinfo));
     task_number=syscall(356,buffer,&size);
     printf("The correct number in buffer is %d \n",task_number);
     get_printfo_result(buffer,0,0,size);
     return 0;
}

