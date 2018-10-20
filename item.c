#include "item.h"

#include <stdio.h>
#include <string.h>

int get_item_by_name(char *name)
{
    for (int i = 0; i < ITEM_COUNT; ++i) {
	if (!strcmp(items[i].name, name))
	    return i;
    }

    return -1;
}
