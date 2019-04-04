#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 5
pthread_mutex_t lock[5];

// The function executed by each thread
void *Philosopher(void *threadid) {
	int tid;
	tid = (int)threadid;

    int left = tid;
    int right = (tid + 1) % 5;
    while(true) {
        int randThink = rand() % 20;
        int randEat = rand() % 20;
        printf("I'm philosopher %d and I'm thinking for %d\n", tid, randThink);
        sleep(randThink);
        printf("I'm philosopher %d and I'm finished thinking, waiting to eat\n", tid);

        switch (tid % 2) {

            case 0:
                pthread_mutex_lock(&lock[left]);
                // printf("I'm philosopher %d and I picked up my left spoon\n", tid);
                pthread_mutex_lock(&lock[right]);
                // printf("I'm philosopher %d and I picked up my right spoon\n", tid);
                break;

            default:
                pthread_mutex_lock(&lock[right]);
                // printf("I'm philosopher %d and I picked up my right spoon\n", tid);
                pthread_mutex_lock(&lock[left]);
                // printf("I'm philosopher %d and I picked up my left spoon\n", tid);
        }

        // Eat using the spoons
        printf("I'm philosopher %d and I'm eating for %d\n", tid, randEat);
        sleep(randEat);
        printf("I'm philosopher %d and I finished eating\n", tid);

        switch (tid % 2) {

            case 0:
                pthread_mutex_unlock(&lock[left]);
                // printf("I'm philosopher %d and I put down my left spoon\n", tid);
                pthread_mutex_unlock(&lock[right]);
                // printf("I'm philosopher %d and I put down my right spoon\n", tid);
                break;

            default:
                pthread_mutex_unlock(&lock[right]);
                // printf("I'm philosopher %d and I put down my right spoon\n", tid);
                pthread_mutex_unlock(&lock[left]);
                // printf("I'm philosopher %d and I put down my left spoon\n", tid);
        }
    }
	// Terminate thread
	pthread_exit(NULL);
}

// Main Function
int main (int argc, char *argv[]) {
    srand(time(NULL));
	// Will create 5 threads, so will need a reference to each
	pthread_t threads[NUM_THREADS];


	long n;
	for(n = 0; n < NUM_THREADS; ++n) {
		// Print
    if (pthread_mutex_init(&lock[n], NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }
		printf("In main: creating thread Philosopher %ld\n", n);

		// Create a thread here, and get the thread to run the PrintHello
		// function. Note the way the parameters are passed
		int rc = pthread_create(&threads[n], NULL, Philosopher, (void *)n);

		// A non-zero return code means something went wrong
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	// Useful to call this in the main to ensure that all threads are kept alive.
	// Otherwise threads that are still going will be automatically terminated
	pthread_exit(NULL);
}



