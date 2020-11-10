# Android_Process_Tree and Linux-Practice
The whole project is developed under Linux OS(Ubuntu 20.04). 
## Android_Process_Tree
In Linux, we can use ps to check the current process. Furthermore, we can use pstree to see the process tree intuitively.
But in Android, we can use ps, but cannot use pstree. 
So I implement a new Android system call with module to process tree information in a depthfirst-search (DFS) order.
Here we use module method rather than revise the kernel code directly, for the fact that in this way we can add system call dynamically.
## Linux Practice
Here I summarise the basic Programm-developing knowledge under Linux. You can see them in Linux_common_commands.txt.
Also, there is a baisc Andriod Debugging Bridge (ADB) usage guidline in ADB_commands.txt. Hope this two files will be helpful for you when you are a novice to Linux system.
## Linux Process Synchronization
– Burger Buddies Problem Description:
* 1.Cooks, Cashiers, and Customers are each modeled as a thread. 
* 2.	Cashiers sleep until a customer is present.
* 3.	A Customer approaching a cashier can start the order process.
* 4.	A Customer cannot order until the cashier is ready.
* 5.	Once the order is placed, a cashier has to get a burger from the rack.
* 6.	If a burger is not available, a cashier must wait until one is made.
* 7.	The cook will always make burgers and place them on the rack.
* 8.	The cook will wait if the rack is full.
* 9.	There are NO synchronization constraints for a cashier presenting food to the customer.
