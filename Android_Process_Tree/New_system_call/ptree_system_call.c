//module definition
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include<linux/string.h>
#include<linux/list.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

MODULE_LICENSE("Dual BSD/GPL");                             //Usually,MODULE LICENSE(), MODULE DESCRIPTION(), and MODULEAUTHOR()—represent details regarding the software license, descriptionof the module, and author

#define __NR_ptreecall 356


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
static int (*oldcall)(void);
char*NInformation="None comment!";

int getDFS(struct prinfo*buf, int*nr,struct task_struct *task,int* currentsize){
    if((*nr)==0||task==NULL) return 0;
    if(*currentsize==(*nr)) return 0;       //if we've made the buffer full
    // get state
    buf[*currentsize].state=task->state;
    //get pid
    buf[*currentsize].pid=task->pid;
    //get uid   searching linux/sched.h: const struct cred __rcu *cred;
    buf[*currentsize].uid=task->cred->uid;

    // get comm
    // method 1, using strcpy, but it's unsafe when destination size is smaller than source
    /*
    if(task->comm!=NULL){
        strcpy(buf[*currentsize].comm,task->comm);
    }
    else{
        strcpy(buf[*currentsize].comm,NInformation);        // if the process do not have comm
    }
    */
   //method 2,using system call: get_task_comm, it's safer
   get_task_comm(buf[*currentsize].comm,task);

    // get parent pid
    if(task->parent!=NULL){
        buf[*currentsize].parent_pid=task->parent->pid;     //struct task_struct __rcu *parent;
    }
    else{
        buf[*currentsize].parent_pid=0;                     
    }

    //get yonger children pid
    // wrong way to judge, for the fact that when a list is empty,list->next==list, it's not when they are both NULL!
    /*
    if(task->children.next==NULL&&task->children.prev==NULL){
        buf[*currentsize].first_child_pid=0;    // this case, the process didn't have children
    }*/
    // use system call: list_empty  I find it's definition in <linux/list.h>:
    //static inline int list_empty(const struct list_head *head)
    if(list_empty(&(task->children))){
        buf[*currentsize].first_child_pid=0;
    }
    else{
        struct task_struct * child_task;
        struct list_head* child_list;
        pid_t min=30000;
        list_for_each(child_list,&task->children){
            child_task=list_entry(child_list,struct task_struct, sibling);
            if(child_task->pid<min)min=child_task->pid;
        }
        buf[*currentsize].first_child_pid=min;         // get the yonger children pid

    }
    //get older sibling pid
    /*
    if(task->sibling.next==task->sibling.prev){
        buf[*currentsize].next_sibling_pid=0;       // this case, according to our requirement, the process didn't have sibling process
    }
    */
    if(list_empty(&(task->sibling))||(task->sibling.next==task->sibling.prev)){
        buf[*currentsize].next_sibling_pid=0;
    }
    else{
        struct task_struct * sibling_task;
        struct list_head* sibling_list;
        sibling_list=task->parent->children.prev;
        sibling_task=list_entry(sibling_list,struct task_struct,sibling);
        if(sibling_task->pid==task->pid){
            buf[*currentsize].next_sibling_pid=0;   //this case, the process didn't have next sibling process,it only have prev process
        }
        else{
            sibling_list=task->sibling.next;
            sibling_task=list_entry(sibling_list,struct task_struct,sibling);
            pid_t min_sibling_pid;
            min_sibling_pid=30000;                          //pid range on linux:0-32768,we set it a large value
            pid_t older_sibling_pid=sibling_task->pid;      // set init value of it to be the next process's pid
            // find the older sibling in the next side
            while(true){
                sibling_task=list_entry(sibling_list,struct task_struct,sibling);
                if(sibling_list==task->parent->children.prev){
                    if((sibling_task->pid < min_sibling_pid) && (sibling_task->pid > task->pid)){
                        min_sibling_pid=sibling_task->pid;
                        older_sibling_pid=min_sibling_pid;
                    }
                    break;
                }
                if((sibling_task->pid > task->pid) && (sibling_task->pid<min_sibling_pid)) {
                    min_sibling_pid=sibling_task->pid;
                    older_sibling_pid=min_sibling_pid;
                }
                sibling_list=sibling_list->next;
            }
            buf[*currentsize].next_sibling_pid=older_sibling_pid;         // get the older sibling pid 
        }
    }
    int num1=0;
    int num2=0;
    int tmp=*currentsize;
    *currentsize=(*currentsize)+1;                      //before we call the sub-routine, currentsize add 1
    if(buf[tmp].first_child_pid==0){
        num1=0;
        buf[tmp].child_position=0;
    }
    else{
        struct task_struct* task1;
        task1=list_entry(task->children.next,struct task_struct,sibling);
        num1=getDFS(buf,nr,task1,currentsize);
        buf[tmp].child_position=1;              //if a process has a child process, it's position must be the next position
    }
    if(buf[tmp].next_sibling_pid==0){
        num2=0;
        buf[tmp].sibling_position=0;
    }
    else{
        struct task_struct* task2;
        task2=list_entry(task->sibling.next,struct task_struct,sibling);
        num2=getDFS(buf,nr,task2,currentsize);              //here, currentsize point to the same place, but it's value add the size in children
        buf[tmp].sibling_position=1+num1;                   //if a process has a sibling process, it's position must be the all the child position+1
    }  
    
    return (1+num1+num2);                         
}
int ptree(struct prinfo*buf, int*nr){
    printk("this is my system process tree call!\n");         //printk() is the kernel equivalent of printf(),but its output is sent to a kernel log buffer whose contents can be read bythe dmesg command.
    if(*nr<0){
        printk("Input error! Size of buffer can't be negative\n");
        return 0;
    }
    else{
        int CurrentSize=0;
        int Result=0;
        int BUFFERsize=*nr;
        struct prinfo *buffer = (struct prinfo*)kmalloc(sizeof(struct prinfo)* (*nr), GFP_KERNEL);     // this kmalloc  in <linux/slab.h>
        read_lock(&tasklist_lock);                          //tasklist_lock is reference in sched.h and is define in fork.c
        Result= getDFS(buffer,&BUFFERsize,& init_task,& CurrentSize);                       //init_task is a global variable which
        read_unlock(&tasklist_lock);
        if(copy_to_user(buf, buffer, sizeof(struct prinfo)*(*nr)) || copy_to_user(nr, &BUFFERsize, sizeof(int))){
            printk(KERN_INFO "copy failed in the kernel\n");
            return -1;                      
        }
        return Result;
    }
}


//module entry point
static int addsyscall_init(void){
    long*syscall=(long*)0xc000d8c4;
    oldcall=(int(*)(void))(syscall[__NR_ptreecall]);
    syscall[__NR_ptreecall]=(unsigned long)ptree;
    printk(KERN_INFO "module load!succeed\n");                  //KERN_INFO  priority flag on which printf output
    return 0;
}
//module exit point
static void addsyscall_exit(void){
    long *syscall =(long*)0xc000d8c4;
    syscall[__NR_ptreecall]=(unsigned long) oldcall;
    printk(KERN_INFO "module exit!ending\n");
}
//properties of module
module_init(addsyscall_init);
module_exit(addsyscall_exit);

