#include "move.h"

#include <stdio.h>
#include <string.h>

int get_move_by_name(char *name)
{
    for (int i = 0; i < MOVE_COUNT; ++i) {
	if (!strcmp(moves[i].name, name))
	    return i;
    }

    return -1;
}
