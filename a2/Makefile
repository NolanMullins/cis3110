OBJ = src/main.o src/list.o src/parse.o src/sim.o
FLAGS = -Wall -Iinclude -std=c99 -Iinclude
EXE = run

all: $(OBJ)
	gcc $(OBJ) $(FLAGS) -o $(EXE)

clean:
	rm $(OBJ) $(EXE)

%.o : %.c
	gcc -c $*.c $(FLAGS) -o $*.o