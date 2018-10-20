#ifndef ABILITY_H
#define ABILITY_H

#define MAX_ABILITY_NAME_LENGTH		30
#define MAX_ABILITY_DESCRIPTION_LENGTH	150

#define ABILITY_COUNT 165

struct AbilityData {
	int id;
	char name[MAX_ABILITY_NAME_LENGTH];
	char description[MAX_ABILITY_DESCRIPTION_LENGTH];
};

struct AbilityData abilities[ABILITY_COUNT];

int get_ability_by_name(char *name);

#endif
