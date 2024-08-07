/*
*   C compiler: gcc -o out thread_basic.c -lpthread
*   C++ compiler: g++ -o out thread_basic.c -lpthread
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h> //sleep

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int shared_variable = 0;

void * threadFunc(void * arg)
{
    int * id = (int *) arg;
    int i = 0;
    for(i=0; i<20; i++)
    {
        pthread_mutex_lock(&mtx);  
        shared_variable++;  
        printf("Thread %d| shared_variable=%d\n",*id,shared_variable); 
        pthread_mutex_unlock(&mtx);

        usleep(100000);
    }
    pthread_exit(NULL);
}

int main()
{
     pthread_t thread1, thread2;
     int id1 = 1;
     int id2 = 2;
     int  iret1, iret2;

    /* Create independent threads each of which will execute function */

     iret1 = pthread_create( &thread1, NULL, threadFunc, (void*) &id1);
     iret2 = pthread_create( &thread2, NULL, threadFunc, (void*) &id2);

     printf("Thread 1 returns: %d\n",iret1);
     printf("Thread 2 returns: %d\n",iret2);
    
     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL); 
    
     return 0;
}
