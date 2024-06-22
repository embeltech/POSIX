#include <stdio.h>
#include <pthread.h>
#include <unistd.h> //sleep

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

int shared_counter = 0;
void * threadFunc(void * arg)
{
    char * msg = (char *) arg;
    int count = 5;
    while(count)
    {
        pthread_mutex_lock(&mtx); //critical section starts
        shared_counter++;
        printf("%s| Count=%d | shared_counter= %d\n",msg,count,shared_counter); 
        pthread_mutex_unlock(&mtx); //critical section ends
        
        count--;
        sleep(1);
    }
    pthread_exit(NULL);
}

int main()
{
     pthread_t thread1, thread2;
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     int  iret1, iret2;

    /* Create independent threads each of which will execute function */

     iret1 = pthread_create( &thread1, NULL, threadFunc, (void*) message1);
     iret2 = pthread_create( &thread2, NULL, threadFunc, (void*) message2);

     printf("Thread 1 returns: %d\n",iret1);
     printf("Thread 2 returns: %d\n",iret2);
    
     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL); 
    
     return 0;
}

