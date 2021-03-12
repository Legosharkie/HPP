CFLAGS=-Wall -g
INCLUDES=-I/opt/X11/include
LDFLAGS=-L/opt/X11/lib -lX11 -lm -lpthread

GameOfLife: GameOfLife.o graphics.o main.o
	gcc -o GameOfLife GameOfLife.o graphics.o main.o $(LDFLAGS)

main.o: main.c graphics.h GameOfLife.h
	gcc $(CFLAGS) $(INCLUDES) -c main.c

GameOfLife.o: GameOfLife.c graphics.h GameOfLife.h
	gcc $(CFLAGS) $(INCLUDES) -c GameOfLife.c

graphics.o: graphics.c graphics.h
	gcc $(CFLAGS) $(INCLUDES) -c graphics.c

clean:
	rm -f ./GameOfLife *.o
