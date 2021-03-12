#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include <sys/time.h>
#include <pthread.h>
#define NUM_THREADS 8
int res;
int N;
int M;

typedef struct thread_data {
	int** currGen;
	int** buffer;
};

struct thread_data* tdata;
pthread_t threads[NUM_THREADS];
/*
Random int generator that generates an integer between 0 and 1 with a weight where the weight specifies the probability of making the return value 1 (one per weight number of chances).
*/
int rand_int(int lower, int upper, int weight);

/*
Calculates the state (number of neighbours) of a given cell with index (i,j).
*/
int getState(int i, int j);


/*
Evolves the current board into the next generation.
*/
void* newGeneration(void* arg);

