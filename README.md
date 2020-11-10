# Android_Process_Tree
The whole project is developed under Linux OS(Ubuntu 20.04). A new Android system call is implemented with module to process tree information in a depthfirst-search (DFS) order. 
## 1_Basic_Test_sysytem_call
This simple C program which calls ptree system call to print the entire process tree (in DFS order). Using tabs to indent children with respect to their parents.
The output format of every process is:
```bash
printf(/* correct number of \t */);
printf("%s,%d,%ld,%d,%d,%d,%d\n", p.comm, p.pid, p.state,p.parent_pid, p.first_child_pid, p.next_sibling_pid, p.uid);
```

