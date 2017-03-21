OBJ = src/main.o src/list.o
FLAGS = -Wall -Iinclude -std=c99 -lpthread
EXE = run

all: $(OBJ)
	gcc $(OBJ) $(FLAGS) -o $(EXE)

clean: 
	rm $(OBJ) $(EXE)
	
%.o : %.c
	gcc -c $*.c $(FLAGS) -o $*.o