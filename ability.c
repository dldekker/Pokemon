#include "ability.h"

#include <stdio.h>
#include <string.h>

int get_ability_by_name(char *name)
{
    for (int i = 0; i < ABILITY_COUNT; ++i) {
	if (!strcmp(abilities[i].name, name))
	    return i;
    }

    return -1;
}
