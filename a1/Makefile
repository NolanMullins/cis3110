OBJQ1 = main.o list.o util.o fnc.o art.o
FLAGS =-Wall -Iinclude -std=c99
EXE = run


all:$(OBJQ1) 
	gcc $(OBJQ1) -o $(EXE) $(FLAGS)
clean:
	rm $(OBJQ1) $(EXE)
run:
	./$(EXE)

main.o: src/main.c 
	gcc -c src/main.c $(FLAGS)
list.o: src/list.c include/list.h
	gcc -c src/list.c $(FLAGS)
util.o: src/util.c include/util.h
	gcc -c src/util.c $(FLAGS)
fnc.o: src/fnc.c include/fnc.h
	gcc -c src/fnc.c $(FLAGS)
art.o: src/art.c include/art.h
	gcc -c src/art.c $(FLAGS)