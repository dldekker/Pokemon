#ifndef ITEM_H
#define ITEM_H

#define MAX_ITEM_NAME_LENGTH 13

#define ITEM_COUNT 450

struct ItemData {
    char name[MAX_ITEM_NAME_LENGTH];
    int price; /* or -1 if cannot be bought/sold */
};

struct ItemData items[ITEM_COUNT];

int get_item_by_name(char *name);

#endif
