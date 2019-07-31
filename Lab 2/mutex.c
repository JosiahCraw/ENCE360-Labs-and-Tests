///
// mutex.c - sum a vector of values using multi-threading and a mutex
// compile with: gcc mutex.c -o mutex --std=c99 -lpthread
//
//


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define NUM_THREADS 16
#define N 10000000


typedef struct {
    int *total;
    int n;

    pthread_mutex_t *lock;
    pthread_t thread;

    int *rc;

} Worker;


void *run_summation(void *ptr)
{
    Worker *worker = (Worker*)ptr;

    for (int i = 0; i < worker->n; ++i) {
	pthread_mutex_lock(worker->lock);
        (*worker->total)++;
	pthread_mutex_unlock(worker->lock);
    }

    return 0;
}



int main()
{

    // Global variable for total summation so far
    int total = 0;

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    Worker workers[NUM_THREADS];


    for (int i = 0; i < NUM_THREADS; ++i) {
        // What would be the problem declaring Worker w here?

        Worker *worker = &workers[i];
        worker->total = &total; // Pass the global total into each thread
        worker->lock = &lock;
        worker->n = N;
	
	int rc = pthread_create(&(worker->thread), NULL, run_summation, worker);

	if (rc) {
            printf("Return Code %d\n", rc);
	} else {
	    printf("Thread %d is good\n", i);
	}
    }
    for (int i=0; i<NUM_THREADS; i++) {
	pthread_join(workers[i].thread, NULL);
    }


    ////////////////////////////////
    // Wait for all the threads we created
    // for(i = ...)
      ////////////////////////////////

    printf("Final total: %d \n", total);

    return 0;
}

