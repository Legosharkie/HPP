#include"graphics.h"
#include"GameOfLife.h"


static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr,"ERROR YOU DUMDUM\n");
    return 0;
  }
  
  /*Setup*/
  Display *dpy = XOpenDisplay(0);
 

  int snum = DefaultScreen(dpy);
  int windowWidth = DisplayWidth(dpy, snum) - 80;
  int windowHeight = DisplayHeight(dpy, snum) - 80; 

  /*Initiate grid*/
  res = atoi(argv[1]);
  N = windowWidth/res; 
  M = windowHeight/res;
  
  tdata = (struct thread_data*)malloc(sizeof(struct thread_data));
  tdata->currGen = (int**)malloc(N*sizeof(int*));
  tdata->buffer = (int**)malloc(N*sizeof(int*));
  
  srand(time(0));
  for (int i = 0; i < N; i++) {
    tdata->currGen[i] = (int*)malloc(M*sizeof(int));
    tdata->buffer[i] = (int*)malloc(M*sizeof(int));
    for (int j = 0; j < M; j++) {
      tdata->currGen[i][j] = rand_int(0,1,20);
      tdata->buffer[i][j] = tdata->currGen[i][j];
    }
  }
  
  /*Graphics routines*/
  //InitializeGraphics(argv[0],windowWidth,windowHeight);
  //SetCAxes(0,1);
  printf("Press 'q' to quit! \n");
  int count = 0;
  double time1 = get_wall_seconds();
  while(++count < 1000) { // !CheckForQuit()
    //ClearScreen();
   
    for (int t = 0; t < NUM_THREADS; t++) {
    	pthread_create(&threads[t],NULL,newGeneration,(void*)&t);
    }
    
    for (int t = 0; t < NUM_THREADS; t++) {
    	pthread_join(threads[t],NULL);
    }
    
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
      tdata->currGen[i][j] = tdata->buffer[i][j];
			if (tdata->currGen[i][j] == 1) {
		  		//DrawRectangle(i*res,j*res,res,res,1);
      	}
      }
    }

   //Refresh();
   //usleep(50000);
  }
  printf("%d generations took %7.3f wall seconds \n",count,get_wall_seconds()-time1);
  for(int i = 0; i < N; i++) {
    free(tdata->currGen[i]);
    free(tdata->buffer[i]);
  }
  free(tdata->buffer);
  free(tdata->currGen);
  
  
  FlushDisplay();
  CloseDisplay();
  return 0;}
