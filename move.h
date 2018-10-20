#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>

#include "type.h"

#define MAX_MOVE_NAME_LENGTH 30
#define MAX_MOVE_ENTRY_LENGTH 200

#define MOVE_COUNT 560

enum MoveCategory {
	PHYSICAL, SPECIAL, STATUS
};

enum MoveTarget {
	TARGET_ADJACENT_SINGLE, TARGET_ADJACENT_USER_SINGLE,
	TARGET_USER_OR_ADJACENT_USER, USER, ALL_USERS,
	ALL_ADJACENT, ADJACENT_FOES_ALL, ALL_EXCEPT_USER,
	ALL_FOES, ALL, USER_AND_RANDOM_ADJACENT_FOE
};

/* Datastructure to store the information about every move */
struct MoveData {
	int index_number;
	int pp;
	int max_pp;
	int power;
	int accuracy;
	enum MoveCategory category;
	int priority;
	enum MoveTarget target;
	int critical_hit;

	bool makes_contact;
	bool affected_by_protect;
	bool affected_by_magic_coat;
	bool affected_by_snatch;
	bool affected_by_mirror_move;
	bool affected_by_kings_rock;

	char name[MAX_MOVE_NAME_LENGTH];
	enum Type type;
	char pokedex_entry[MAX_MOVE_ENTRY_LENGTH];
};

const struct MoveData moves[MOVE_COUNT];

int get_move_by_name(char *name);

#endif
