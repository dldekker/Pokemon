#ifndef POKEMON_H
#define POKEMON_H

#include "move.h"
#include "type.h"
#include "ability.h"
#include "item.h"

#define MAX_POKEDEX_ENTRY_LENGTH 400

#define GENDER_CHANCE_MALE	0
#define GENDER_CHANCE_FEMALE	1

#define HP	0
#define ATK	1
#define DEF	2
#define SP_ATK	3
#define SP_DEF	4
#define SPD	5

enum LevelingRate {
	EXP_ERRATIC, EXP_FAST, EXP_MEDIUM_FAST, EXP_MEDIUM_SLOW, EXP_SLOW, EXP_FLUCTUATING
};

#define EGG_NONE_GROUP		0
#define EGG_MONSTER_GROUP	1
#define EGG_WATER_1_GROUP	2
#define EGG_BUG_GROUP		3
#define EGG_FLYING_GROUP	4
#define EGG_FIELD_GROUP		5
#define EGG_FAIRY_GROUP		6
#define EGG_GRASS_GROUP		7
#define EGG_HUMAN_LIKE_GROUP	8
#define EGG_WATER_3_GROUP	9
#define EGG_MINERAL_GROUP	10
#define EGG_AMORPHOUS_GROUP	11
#define EGG_WATER_2_GROUP	12
#define EGG_DITTO_GROUP		13
#define EGG_DRAGON_GROUP	14
#define EGG_UNDISCOVERED_GROUP	15

#define EVOLUTION_NONE		0
#define EVOLUTION_LEVEL		1
#define EVOLUTION_MOVE		2
#define EVOLUTION_HAPPINESS	3
#define EVOLUTION_TRADE		4
#define EVOLUTION_TRADE_ITEM	5
#define EVOLUTION_MOSSY_ROCK	6
#define EVOLUTION_ICY_ROCK	7
#define EVOLUTION_MAGNETIC_FIELD	8
#define EVOLUTION_ITEM			9
#define EVOLUTION_ITEM_MALE		21
#define EVOLUTION_ITEM_FEMALE		22
#define EVOLUTION_BEAUTY		10
#define EVOLUTION_LEVEL_MALE		11
#define EVOLUTION_LEVEL_FEMALE		12
#define EVOLUTION_LEVEL_RANDOM		13
#define EVOLUTION_LEVEL_FEMALE_OUTSIDE	14
#define EVOLUTION_LEVEL_FEMALE_INSIDE	15
#define EVOLUTION_LEVEL_FEMALE_CAVES	16
#define EVOLUTION_LEVEL_EMPTY_SPOT	17
#define EVOLUTION_LEVEL_ATK_LARGER_DEF	18
#define EVOLUTION_LEVEL_ATK_SMALLER_DEF	19
#define EVOLUTION_LEVEL_ATK_EQUAL_DEF	20

#define NONE		0
#define MORNING		1
#define DAYTIME		2
#define NIGHTTIME	3

enum Nature {
	HARDY,	LONELY, ADAMANT,NAUGHTY,BRAVE,
	BOLD,	DOCILE,	IMPISH,	LAX,	RELAXED,
	MODEST,	MILD,	BASHFUL,RASH,	QUIET,
	CALM,	GENTLE,	CAREFUL,QUIRKY,	SASSY,
	TIMID,	HASTY,	JOLLY,	NAIVE,	SERIOUS
};

enum Status {
	NO_STATUS, ASLEEP, POISONED, BURNED, FROZEN, PARALYZED, TOXIC
};

struct Pokemon {
	int pokemon_id;
	int held_item_id;
	int experience;
	int ability_id;
	int ev[6];

	struct {
		int id;
		int pp;
		int pp_ups;
	} move[4];

	int iv[6];
	char nickname[11];

	enum Nature nature;

	int stat[6];
	int current_hp;
	int level;
	enum Status status;
	
	bool egg;
	int cycles_to_hatch;
};

struct PokemonData {
	int id;
	char name[11];
	enum Type types[2];
	char abilities[2][MAX_ABILITY_NAME_LENGTH];
	char hidden_ability[MAX_ABILITY_NAME_LENGTH];
	float gender_ratios[2];
	int catch_rate;

	int egg_group[2];
	int hatch_time[2];
	int height; /* In meter */
	int weight; /* In kilogram */
	int base_exp_yield;
	enum LevelingRate leveling_rate;
	int ev_yield[6];
	int base_friendship;
	int base_stats[6];
	char pokedex_entry[MAX_POKEDEX_ENTRY_LENGTH];

	struct Evolution {
		int evolution_type;
		int level;
		char item[15]; //Can be pokemon or item
		int time;
	} evolutions[7];
		
	struct LearnableMove {
		char name[MAX_MOVE_NAME_LENGTH];
		int egg_move;
		int level;
		int TMHM;
	} learnset[125];
};

/* Creates a new pokemon at a certain level. */
void generate_pokemon(struct Pokemon *pokemon, int pokemon_id, int level);

/* Print a readout of all the pokemon information. */
void print_pokemon(struct Pokemon *pokemon);

//TODO: Add Evolution data id's
//TODO: Feebas Evolution (change max beauty)

struct PokemonData pokemons[650];


#endif
