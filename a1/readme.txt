/****************************************
 * Nolan Mullins
 * 0939720
 * 30/01/2017
 * cis3110 - A1
 ****************************************/


**********************************
Problem
**********************************
    This program will implement a simple unix shell, it will run command line input 
along with some custom commands. It will be able to run processes in t he background using
the & and also direct i/o to files using the <>. Typing add *numbers will add the numbers 
and print the sums. Typing arg will list the arugments. Typing art will print out some
random acsii art.


**********************************
Algorithm
**********************************
	For parsing the initial line I used a state based parser and through the arguments
into a list for later use. The words are space delimited except for the case of strings.
After parsing the arugments are passed to runCmd where some info is gathered on the command
and then passed into a child process which acts on the command. My custom command will generate
a random number and then print a coressponding ascii picture. 

**********************************
Guide
**********************************
Compile
type: make
output: run

Running
type: ./run

Using the art command
enter "art" and a random picture will printed
Ex)
> art
__        __   _
\ \      / /__| | ___ ___  _ __ ___   ___
 \ \ /\ / / _ \ |/ __/ _ \| '_ ` _ \ / _ \
  \ V  V /  __/ | (_| (_) | | | | | |  __/
   \_/\_/ \___|_|\___\___/|_| |_| |_|\___|


**********************************
Assumptions
**********************************
	Arugments passed to the add command are infact valid numbers. A
hex number will begin with 0x. If an invalid number is found as an arg 
the program will not add it.
