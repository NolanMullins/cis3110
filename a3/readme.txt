 ****************************************
 * Nolan Mullins
 * 0939720
 * 26/03/2017
 * cis3110 - A3
 ****************************************


**********************************
Problem
**********************************
    Dining Philosophers and Simple Memory Management

**********************************
Algorithm
**********************************
	P1 literally just throws a struct on a thread, put a mutex on the forks and thats it
	P2 used a bit map to represent the memory. Used first fit, Next fit, best fit, worst fit for laoding
		procs into

**********************************
Guide
**********************************
	Compile
	type: make
	output: p1, p2

	Running
	type: 	./p1 #philosophers #eats
			./p2 filePath

**********************************
Assumptions
**********************************
	- you want to run all 4 memory sims at once
