OBJ1 = src/p1.o src/list.o
OBJ2 = src/p2.o src/list.o
FLAGS = -Wall -Iinclude -std=c99 -lpthread
EXE1 = p1
EXE2 = p2

all: p1 p2
	
p2: $(OBJ2)
	gcc $(OBJ2) $(FLAGS) -o $(EXE2)

p1: $(OBJ1)
	gcc $(OBJ1) $(FLAGS) -o $(EXE1)

clean: 
	rm $(OBJ1) $(EXE1) $(OBJ2) $(EXE2)
	
%.o : %.c
	gcc -c $*.c $(FLAGS) -o $*.o