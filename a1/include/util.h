#ifndef __MULLINSN_UTIL__
#define __MULLINSN_UTIL__

#include "list.h"

//returns a malloced string with data copied in it
char* strgen(char* data);
//converts a line into a list of words
List* parseLine(char* line);

#endif