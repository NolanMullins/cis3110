/****************************************
 * Nolan Mullins
 * 0939720
 * 24/02/2017
 * cis3110 - A2
 ****************************************/

**********************************
Questions
**********************************
i)		No my program does not include switch overhead for the first ready state to running state transition.
			I have done this because I am assuming on startup the first process in the queue will be loaded and
			ready when the cpu is started. 

ii) 	Yes the program will include switch overhead becuase a new thread will have to be moved from the ready state
			to running state. Also the previous thread will have to be unloaded to make space for the new thread.

iii) 	No the program will not include switch overhead if a thread moves from ready to blocked and the queue is
			emtpy. I do this because the thread will just be given another cpu burst and such no overhead will
			occur since its the same thread as before.

iv) 	Yes and No the program may include switch overhead if a thread is interrupted and the queue is either emtpy or
			the thread has the highest priority. This is because once a thread is interrupted due to a time slice
			the thread has recieved its run time and is now moved the back of the queue. If the queue is empty then
			the thread will be given another cpu burst (refer to iii). If the queue is not empty then the thread will
			be pushed to the back of the queue and a new thread will be loaded which will require switch overhead.

**********************************
Problem
**********************************
    This program will simulate a cpu scheduling algorithm that will execute a group of multithreaded processes in an
OS. The program wil implement first come first serve and and round robin simulations.


**********************************
Algorithm
**********************************
	The two algorithms used are first come first serve and Round Robin.
For the first come first serve the cpu will run each thread in sequencal order based upon when
it arrived. The cpu will run the entire thread from start to finish with no interrupts.
For the Round Robin the cpu will give each thread smaller burst of cpu time of 100u.

**********************************
Guide
**********************************
Compile
type: make
output: run

Running
type: ./run [args]

**********************************
Assumptions
**********************************
	I'm assuming the cpu can only work on one thread at a time.
	If no quantum is specified or an invalid number, the quantum is set 
		to 100