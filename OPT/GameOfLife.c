#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include"graphics.h"
#include"GameOfLife.h"
#include <sys/time.h>
#include <pthread.h>

int res;
int N;
int M;

int** grid_Buffer;
int** grid_PrevGen;

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

int rand_int(int lower, int upper, int weight) {
  upper = upper * (weight-1);
  int res = (rand() % (upper - lower + 1)) + lower;
  if (res == 1)
    return res;
  else
    return 0;
}
	
int getState(int i, int j, int*** currGen) {
	int state = 0;
	int** points = *currGen; 
	int ii;
	int jj;
	//double time1 = get_wall_seconds();
	for (int ki = -1; ki < 2; ki++) {
		for (int kj = -1; kj < 2; kj++) {
			ii = (i + ki + N) % N;
			jj = (j + kj + M) % M;
			state += points[ii][jj];
		}
	}
	
	state -= points[i][j]; // subtracting own cell
	//printf("Took %7.3f Wall seconds \n",get_wall_seconds() - time1);
	return state;
}

void newGeneration(int*** currGen, int** nextGen) {
	int state = 0;
	int curr = 0;
	int** points = *currGen;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			state = getState(i,j,currGen);
			//printf("Current state: %d \n",state);
			curr = points[i][j];
			
			// Generates the new generation according to the rules
			// Dies by loneliness
			if (curr && state < 2)
				nextGen[i][j] = 0;
			
			// Dies by overpopulation
			else if (curr && state > 3)
				nextGen[i][j] = 0;
				
			// Rebirth
			else if (!curr && state == 3) 
				nextGen[i][j] = 1;
				
			// Nothing happens
			else
				nextGen[i][j] = curr;				
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			points[i][j] = nextGen[i][j];
		}
	}

}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr,"ERROR YOU DUMDUM\n");
    return 0;
  }

  Display *dpy = XOpenDisplay(0);

  int snum = DefaultScreen(dpy);
  int windowWidth = DisplayWidth(dpy, snum) - 80;
  int windowHeight = DisplayHeight(dpy, snum) - 80; 

  res = atoi(argv[1]);
  N = windowWidth/res; // atoi(argv[1]);
  M = windowHeight/res;
  grid_PrevGen = (int**)malloc(N*sizeof(int*));
  grid_Buffer = (int**)malloc(N*sizeof(int*));

  srand(time(0));
  for (int i = 0; i < N; i++) {
    grid_PrevGen[i] = (int*)malloc(M*sizeof(int));
    grid_Buffer[i] = (int*)malloc(M*sizeof(int));
    for (int j = 0; j < M; j++) {
      if (i == 0 || i == N-1 || j == 0 || j == M-1) {
	//grid_PrevGen[i][j] = 0;
      }
      else {
	grid_PrevGen[i][j] = rand_int(0,1,10);
      }
      grid_Buffer[i][j] = grid_PrevGen[i][j];
      // printf("grid[%d,%d] = %d\n",i,j,grid_PrevGen->board[i][j]);
    }
  }

  InitializeGraphics(argv[0],windowWidth,windowHeight);
  SetCAxes(0,1);
  while(!CheckForQuit()) {
    ClearScreen();
    newGeneration(&grid_PrevGen, grid_Buffer);
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
	if (grid_PrevGen[i][j] == 0) {
	}
	else
	  DrawRectangle(i*res,j*res,res,res,1);	//printf("#");
      }
    }
    Refresh();
    usleep(50000);
    //system("clear");
  }
  for(int i = 0; i < N; i++) {
    free(grid_PrevGen[i]);
    free(grid_Buffer[i]);
  }
  free(grid_Buffer);
  free(grid_PrevGen);
  
  
  FlushDisplay();
  CloseDisplay();
  return 0;}
