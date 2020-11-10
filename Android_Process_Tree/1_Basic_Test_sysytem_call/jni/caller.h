#ifndef HELLOHEADER_H_ 
#define HELLOHEADER_H_ 
#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <linux/sched.h> 
struct prinfo{
    pid_t parent_pid;
    pid_t pid;
    pid_t first_child_pid;
    pid_t next_sibling_pid;
    long state;
    long uid;
    char comm[64];
    int child_position;
    int sibling_position;
};
void get_printfo_result(struct prinfo*buf,int current_position,int Indent_Num,int size){
    if(current_position>=size)return;
    //get correct indentation
    int i=0;
    for(i=0;i<Indent_Num;++i)
        printf("\t");
    //print
    int tmp=0;
    printf("%s,%d,%ld,%d,%d,%d,%d\n",buf[current_position].comm,
    buf[current_position].pid,
    buf[current_position].state,
    buf[current_position].parent_pid,
    buf[current_position].first_child_pid,
    buf[current_position].next_sibling_pid,
    buf[current_position].uid);
    //if have child, print child
    if(buf[current_position].child_position!=0){
        tmp=current_position+buf[current_position].child_position;
        get_printfo_result(buf,tmp,Indent_Num+1,size);
    }
    //if have siling,print sibling.
    if(buf[current_position].sibling_position!=0){
        tmp=current_position+buf[current_position].sibling_position;
        get_printfo_result(buf,tmp,Indent_Num,size);
    }
}
#endif		/*HELLOHEADER_H_*/
