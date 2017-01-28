#include <stdio.h>
#include <string.h>
#include "util.h"


char* strgen(char* data)
{
	char* ptr = malloc(sizeof(char)*(strlen(data)+1));
	strcpy(ptr, data);
	return ptr;
}

//hidden
char* append(char* line, char c)
{
	int len = strlen(line);
	line[len] = c;
	line[len+1] = '\0';
	return line;
}

List* parseLine(char* line)
{
	int size = strlen(line);
	char word[size+1];
	word[0] = '\0';
	List* list = init();
	for (int a = 0; a < size; a++)
	{
		if (line[a] == '"')
		{
			while (line[++a] != '"')
			{
				append(word, line[a]);
			}
		}
		else if (line[a] == ' ' || line[a] == '\n' || line[a] == '\t' || line[a] == '\r')
		{
			listAdd(list, strgen(word));
			word[0] = '\0';
		}
		else
			append(word, line[a]);
	}
	return list;
}
