OBJ = src/p1.o src/list.o
FLAGS = -Wall -Iinclude -std=c99 -lpthread
EXE1 = p1

all: $(OBJ)
	gcc $(OBJ) $(FLAGS) -o $(EXE1)

clean: 
	rm $(OBJ) $(EXE1)
	
%.o : %.c
	gcc -c $*.c $(FLAGS) -o $*.o