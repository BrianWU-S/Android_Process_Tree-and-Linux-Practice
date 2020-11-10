#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

//parent and child process are concurrent process that using the same code instructions
int main()
{
pid_t pid;
/* fork a child process */
pid = fork();               //one of the two processes typically uses the exec() system call to replace the process’s memory space with a new program.
if (pid < 0) { /* error occurred */
    fprintf(stderr, "Fork Failed");
    return 1;
}
else if (pid == 0) { /* child process */
    pid_t child=getpid();
    printf("518021910665Child %d \n",child);
    execl("/data/misc/callerARM","callerARM",NULL);                //The child process overlays its address space with command /bin/ls using the execlp()system call (a version of the exec() system call).
}
else { /* parent process */
/* parent will wait for the child to complete */
    pid_t parent=getpid();
    printf("518021910665Parent %d \n",parent);
    pid_t Cpid;
    int status;
    Cpid=wait(&status);
    printf("Child return status:%i\n",status);
    printf("Child Complete");
}
return 0;
}
