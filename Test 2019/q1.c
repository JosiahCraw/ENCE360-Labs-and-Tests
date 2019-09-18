#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>

#define NUM_WRITERS 10
#define NUM_READERS 2
#define BUFFER_SIZE 10

sem_t writers;

typedef struct {
    // Read and write semaphores for our channel
    pthread_mutex_t wr_lock;
    pthread_mutex_t rd_lock;
    sem_t empty;
    sem_t full;

    // Global shared data
    int write_index;
    int read_index;
    int global_data[BUFFER_SIZE];
} Channel;

typedef struct {
    int id;
    Channel* channel;
} ThreadArg;


void read_data();
void write_data();
void init_channel();

int main()
{
    pthread_t threads[NUM_WRITERS+NUM_READERS];
    ThreadArg arg[NUM_WRITERS+NUM_READERS];
    Channel channel;
    int result;
    int* status;
    int i;

    srand(2018);
    sem_init(&writers, 0, 0);
    init_channel(&channel);

    for(i = 0; i < NUM_WRITERS; i++) {
        arg[i].id = i;
        arg[i].channel = &channel;
        sem_post(&writers);
        pthread_create(&threads[i], NULL, (void*)&write_data, &arg[i]);
    }
    for(i = 0; i < NUM_READERS; i++) {
        arg[i].id = i;
        arg[i].channel = &channel;
        pthread_create(&threads[NUM_WRITERS+i], NULL, (void*)&read_data, &arg[i]);
    }

    // Wait for threads to finish before continuing
    // Ensure that "result" is the sum of the return codes from the readers
    // Place your code between the lines of //
    ///////////////////////////////////////////////
    for (int i = 0; i < NUM_WRITERS; i++) {
      pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < NUM_READERS; i++) {
      pthread_join(threads[NUM_WRITERS+i], (void **)status);
      result += *status;
    }
    ///////////////////////////////////////////////

    printf("Result is: %d\n", result);
    exit(0);
}

void write_data(ThreadArg* arg)
{
    int count=0;
    int id = arg->id;
    Channel* channel = arg->channel;

    printf("New writer (%03d)\n", id);

    while(1) {
        usleep(rand()%10000);

        // Enter critical region for writer
        // Place your code between the lines of //
        ///////////////////////////////////////////////
        sem_wait(&channel->empty);
        ///////////////////////////////////////////////
        sem_wait(&channel->full);
        channel->global_data[channel->write_index] = (id+1) * (count+1);
        printf("W(%03d) channel %06d\n",  id, channel->global_data[channel->write_index]);
        channel->write_index = (channel->write_index + 1) % BUFFER_SIZE;
        sem_post(&channel->empty);
        // Exit critical region for writer
        // Place your code between the lines of //
        ///////////////////////////////////////////////
        sem_post(&channel->full);
        ///////////////////////////////////////////////
        count++;

        if (count == 10) {
            pthread_exit(NULL);
        }
    }
}

void read_data(ThreadArg* arg)
{
    int result=0;
    int rc1, rc2;
    int* returnval;
    int adder=0;
    int id = arg->id;
    Channel* channel = arg->channel;

    printf("New Reader (%03d)\n", id);

    while(1) {
        // Enter critical region for reader
        // Place your code between the lines of //
        ///////////////////////////////////////////////
        sem_wait(&channel->full);
        ///////////////////////////////////////////////
        sem_getvalue(&channel->empty, &rc1);
        sem_getvalue(&writers, &rc2);
        if (rc1 == 0 && rc2 == 0) {
            pthread_mutex_unlock(&channel->rd_lock);
            break;
        }
        sem_wait(&channel->empty);
        result += channel->global_data[channel->read_index];
        channel->read_index = (channel->read_index + 1) % BUFFER_SIZE;
        sem_post(&channel->full);
        // Exit critical region for reader
        // Place your code between the lines of //
        ///////////////////////////////////////////////
        sem_post(&channel->empty);
        ///////////////////////////////////////////////
        usleep(rand()%20000);
    }

    // Ensure on exit of thread that it returns the value of "result"
    // Place your code between the lines of //
    ///////////////////////////////////////////////

    ///////////////////////////////////////////////
}



void init_channel(Channel *channel)
{

    // Initialise count of the read semaphore to 0 (there's nothing to read yet)
    // Place your code between the lines of //
    ///////////////////////////////////////////////
    sem_init(&channel->empty, 0, 0);
    sem_init(&channel->full, 0, 1);
    ///////////////////////////////////////////////

    channel->read_index = 0;
    channel->write_index = 0;
}
