/*
* https://rextester.com/l/cpp_online_compiler_gcc
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h> //sleep

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_cond = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

/*
 A condition variable must always be associated with a mutex to avoid a race condition 
 created by one thread preparing to wait and another thread which may signal the condition 
 before the first thread actually waits on it resulting in a deadlock. 
*/

int shared_counter = 0;
void * threadFunc1(void * arg)
{
    char * msg = (char *) arg;
    int count = 10;
    while(count)
    {
        //-------------- Wait Condition-----------------
        // make Thread 1 wait till Thread 2 increaments shared_counter till 5. then both threads continue.
        pthread_mutex_lock(&mtx_cond);
        if(shared_counter < 5)
        {
            printf("T1 waiting\n");
            pthread_cond_wait(&cond_var, &mtx_cond);
        }        
        pthread_mutex_unlock(&mtx_cond);
        
        
        pthread_mutex_lock(&mtx); //critical section starts
        shared_counter++;
        printf("%s| Count=%d | shared_counter= %d\n",msg,count,shared_counter); 
        pthread_mutex_unlock(&mtx); //critical section ends
        
        count--;
        usleep(100000);//IMP as other thread should get chance to run. else single thread will run all the time
    }
    printf("%s exiting\n",msg);
    pthread_exit(NULL);
}

void * threadFunc2(void * arg)
{
    char * msg = (char *) arg;
    int count = 10;
    while(count)
    {
        //-------------- signal Condition-----------------
        // make Thread 1 wait till Thread 2 increaments shared_counter till 5. then both threads continue.
        pthread_mutex_lock(&mtx_cond);
        if(shared_counter == 5)
        {
            printf("T2 signalling\n");
            pthread_cond_signal(&cond_var);
        }        
        pthread_mutex_unlock(&mtx_cond);
        
        
        pthread_mutex_lock(&mtx); //critical section starts
        shared_counter++;
        printf("%s| Count=%d | shared_counter= %d\n",msg,count,shared_counter); 
        pthread_mutex_unlock(&mtx); //critical section ends
        
        count--;
        usleep(100000); //IMP as other thread should get chance to run. else single thread will run all the time
    }
    printf("%s exiting\n",msg);
    pthread_exit(NULL);
}

int main()
{
     pthread_t thread1, thread2;
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     int  iret1, iret2;

    /* Create independent threads each of which will execute function */

     iret1 = pthread_create( &thread1, NULL, threadFunc1, (void*) message1);
     sleep(1);
     iret2 = pthread_create( &thread2, NULL, threadFunc2, (void*) message2);

     printf("Thread 1 returns: %d\n",iret1);
     printf("Thread 2 returns: %d\n",iret2);
    
     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL); 
    
     return 0;
}

