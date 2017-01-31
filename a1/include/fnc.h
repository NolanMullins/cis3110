#ifndef __MULLINSN_FNC__
#define __MULLINSN_FNC__

//run a command stored in cmd and add the children ids to children if the process was left running
int runCmd(List* cmd, List* children);
//convert a linked list to string array
char** conv(List* cmd);

#endif