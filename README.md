# Android_Process_Tree
The whole project is developed under Linux OS(Ubuntu 20.04). A new Android system call is implemented with module to process tree information in a depthfirst-search (DFS) order.

##  Basic Test for new sysytem call
This simple C program which calls ptree system call to print the entire process tree (in DFS order). Using tabs to indent children with respect to their parents.
The output format of every process is:
```bash
printf(/* correct number of \t */);
printf("%s,%d,%ld,%d,%d,%d,%d\n", p.comm, p.pid, p.state,p.parent_pid, p.first_child_pid, p.next_sibling_pid, p.uid);
```
## Changed Tree test for new system call
Generate a new process and output“StudentIDParent” with PID, then
generates its children process, which output “StudentIDChild” with PID.
Then use execl to execute ptree in the child process，and examine the connection between above two processes.

## Linux_Process_Synchronization
– Burger Buddies Problem
* 1. 	Cooks, Cashiers, and Customers are each modeled as a thread.
* 2.	Cashiers sleep until a customer is present.
* 3.	A Customer approaching a cashier can start the order process.
* 4.	A Customer cannot order until the cashier is ready.
* 5.	Once the order is placed, a cashier has to get a burger from the rack.
* 6.	If a burger is not available, a cashier must wait until one is made.
* 7.	The cook will always make burgers and place them on the rack.
* 8.	The cook will wait if the rack is full.
* 9.	There are NO synchronization constraints for a cashier presenting food to the customer.

# Each part's Command line result is saved in Test_result.txt separately. 
