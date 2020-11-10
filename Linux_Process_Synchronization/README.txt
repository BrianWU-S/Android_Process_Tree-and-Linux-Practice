– Burger Buddies Problem

1. 	Cooks, Cashiers, and Customers are each modeled as a thread.
2 .	Cashiers sleep until a customer is present.
3.	A Customer approaching a cashier can start the order process.
4.	A Customer cannot order until the cashier is ready.
5.	Once the order is placed, a cashier has to get a burger from the rack.
6 .	If a burger is not available, a cashier must wait until one is made.
7.	The cook will always make burgers and place them on the rack.
8.	The cook will wait if the rack is full.
9.	There are NO synchronization constraints for a cashier presenting food to the customer.