#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include"graphics.h"
#include"GameOfLife.h"
#include <sys/time.h>
#include <pthread.h>


/*
Random int generator that generates an integer between 0 and 1 with a weight where the weight specifies the probability of making the return value 1 (one per weight number of chances).
*/
int rand_int(int lower, int upper, int weight) {
  upper = upper * (weight-1);
  int res = (rand() % (upper - lower + 1)) + lower;
  if (res == 1)
    return res;
  else
    return 0;
}

/*
Calculates the state (number of neighbours) of a given cell with index (i,j).
*/
int getState(int i, int j) {
	int state = 0;
	int ii;
	int jj;
	//double time1 = get_wall_seconds();
	for (int ki = -1; ki < 2; ki++) {
		for (int kj = -1; kj < 2; kj++) {
			ii = (i + ki + N) % N;
			jj = (j + kj + M) % M;
			state += tdata->currGen[ii][jj];
		}
	}
	
	state -= tdata->currGen[i][j]; // subtracting own cell
	//printf("Took %7.3f Wall seconds \n",get_wall_seconds() - time1);
	return state;
}


/*
Evolves the current board into the next generation.
*/
void* newGeneration(void* arg) {
	int id = *((int*)arg);
	printf("%d\n",id);
	int state = 0;
	//data->currGen;
	//data->buffer;
	for (int i = id; i < N; i+= NUM_THREADS) {
		for (int j = 0; j < M; j++) {
			state = getState(i,j);
			
			/* 
			Generates the new generation according to the rules
			*/
			
			// Dies by loneliness
			if (tdata->currGen[i][j] && state < 2)
				tdata->buffer[i][j] = 0;
			
			// Dies by overpopulation
			else if (tdata->currGen[i][j] && state > 3)
				tdata->buffer[i][j] = 0;
				
			// Rebirth
			else if (!(tdata->currGen[i][j]) && state == 3) 
				tdata->buffer[i][j] = 1;
				
			// Nothing happens
			else
				tdata->buffer[i][j] = tdata->currGen[i][j];				
		}
	}
	
	return NULL;
}

