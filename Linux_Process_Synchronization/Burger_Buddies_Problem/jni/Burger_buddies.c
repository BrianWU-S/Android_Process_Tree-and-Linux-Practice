#include"Burger_buddies.h"

sem_t empty;
sem_t full;
sem_t getBurger;
sem_t serve;
sem_t order;
//I've also try to use Mutex to finish it, it also works
//pthread_mutex_t CookMutex;
//pthread_mutex_t CashierMutex;
sem_t CookMutex;
sem_t CashierMutex;
//pthread_mutex_t mutex;                //lock same resource count between cook and cashier,just de-comment it if you want to see that. 
int * Buffer;
int BufferSize;
int Count=0;
int in=0;
int out=0;

void* Cook(void*CookArg){
     int *p=((int *)CookArg);
     int N=*p;      //N is the cook identifier
     while(1){
          sem_wait(&full);          // if the buffer is full, cooks need to wait
          double n = 2*rand() / (RAND_MAX);            // simulate the time to make a burger
          sleep(n);
          //pthread_mutex_lock(&CookMutex);              // enter each cooks critical section, put its burger into buffer
          sem_wait(&CookMutex);
          Buffer[in]=1;
          in=(in+1) % BufferSize;
          //pthread_mutex_lock(&mutex);                  // try to simulate same resource count between cook and cashier 
          //Count++;
          printf("Cook[ %d ] make a burger\n",N);
          //pthread_mutex_unlock(&mutex);
          //pthread_mutex_unlock(&CookMutex);
          sem_post(&CookMutex);
          sem_post(&empty);         //  sent that the buffer now it's not empty
     }
}

void* Cashing(void * CashArg){
     int *p=((int *)CashArg);
     int N=*p;      //N is the cashier identifier
     while(1){
          sem_wait(&order);         // if the customer do not sent a order , just wait
          printf("Cashier[%d ] accept an order\n",N);
          sem_wait(&empty);         // if the buffer is empty now, just wait
          double n = 0.5*rand() / (RAND_MAX);            // simulate the time to get a burger
          sleep(n);
          sem_wait(&CashierMutex);
          //pthread_mutex_lock(&CashierMutex);           // enter the cashier's critical section, take a burger from the buffer
          Buffer[out]=0;
          out=(out+1) % BufferSize;
          //pthread_mutex_lock(&mutex);                  // try to simulate same resource count between cook and cashier 
          //Count--;
          //pthread_mutex_unlock(&mutex);
          //pthread_mutex_unlock(&CashierMutex);
          sem_post(&CashierMutex);        
          printf("Cashier[%d ] take a burger to customer\n",N);
          sem_post(&full);                         // tell the cook he can make one more burger
          sem_post(&serve);                        // tell the customers that there is one more cashier
          sem_post(&getBurger);                    // tell the waiting customer that his burger is ready and he can take it
     }
}

void* Buying(void* BuyArg){
     int *p=((int *)BuyArg);
     int N=*p;      //N is the customer identifier
     printf("Customer[%d ] come\n",N);
     sem_wait(&serve);
     sem_post(&order);
     sem_wait(&getBurger);
     printf("Customer[%d ] leave\n",N);
}

int main(int argc, char *argv[]){ 
     printf("Begin run\n");

     // get argument
     int cook_num = atoi(argv[1]);
     int cashier_num = atoi(argv[2]);
     int customer_num = atoi(argv[3]);
     int size = atoi(argv[4]);
     if(cook_num<=0 ||cashier_num<=0 ||customer_num<=0 ||size<=0){
          printf("Input Error!");
          return 1;
     }
     printf("Cooks[%d], Cashiers[%d], Customers[%d], Buffer size: %d \n", cook_num, cashier_num, customer_num, size);
     
     // init semaphore
     srand(time(NULL));                 // srand random seed here
     int s1 = sem_init(&empty, 0, 0);
     int s2 = sem_init(&full, 0, size);
     int s3 = sem_init(&getBurger, 0, 0);
     int s4 = sem_init(&serve, 0,cashier_num);
     int s5 = sem_init(&order, 0, 0);
     int s6=sem_init(&CookMutex,0,1);
     int s7=sem_init(&CashierMutex,0,1);
     if(s1 || s2 || s3 || s4 || s5 || s6 ||s7){
          printf("Semaphore initialization failed!\n");
          return 1;
     }

     //another way to do that: nit mutex lock
     //pthread_mutex_init(&CookMutex,NULL);		// init , use default attribute 
     //pthread_mutex_init(&CashierMutex,NULL);		// init , use default attribute 
     //pthread_mutex_init(&mutex,NULL);		// init , use default attribute 
     printf("init mutex lock\n");
     // init Buffer
     Buffer = (int*)malloc(sizeof(int) * size);
     BufferSize=size;
     printf("init Buffer\n");
     // init threads
     pthread_t* cooks = (pthread_t*)malloc(sizeof(pthread_t) * cook_num);
     pthread_t* cashiers = (pthread_t*)malloc(sizeof(pthread_t) * cashier_num);
     pthread_t* customers = (pthread_t*)malloc(sizeof(pthread_t) * customer_num);
     printf("init threads\n");
     int * cookIdentifier=(int*)malloc (sizeof(int)*cook_num);
     int * cashierIdentifier=(int*)malloc (sizeof(int)*cashier_num);
     int * customerIdentifier=(int*)malloc (sizeof(int)*customer_num);
     int i=0;
     printf("init threads Identifier\n");
     //create cook threads
     for (i = 0; i < cook_num; i++){
          cookIdentifier[i]=i;
          pthread_create(&cooks[i], NULL, &Cook,&cookIdentifier[i]);
     }
     //create cashier threads
     for (i = 0; i < cashier_num; i++){
          cashierIdentifier[i]=i;
          pthread_create(&cashiers[i], NULL, &Cashing, &cashierIdentifier[i]);
     }
     //create customer threads
     for (i = 0; i < customer_num; i++){
          customerIdentifier[i]=i;
          pthread_create(&customers[i], NULL, &Buying, &customerIdentifier[i]);
          double n = 3*rand()/(RAND_MAX);            // simulate the time for customers to come
          sleep(n);
     }
     //join all the customers and then return
     for(i=0;i<customer_num;i++){
	     pthread_join(customers[i],NULL);
     }
     return 0;
}

