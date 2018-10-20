#include "pokemon.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int get_experience_at_level(int pokemon_id, int level);
static int get_ability(int pokemon_id);
static void generate_moves(struct Pokemon *pokemon, int level);
static void calculate_stats(struct Pokemon *pokemon);
static float get_nature_multiplier(enum Nature nature, int stat);
static char *get_nature_string(enum Nature nature);

void generate_pokemon(struct Pokemon *pokemon, int pokemon_id, int level)
{
    pokemon->pokemon_id = pokemon_id;
    pokemon->held_item_id = 0;

    pokemon->experience = get_experience_at_level(pokemon_id, level);

    pokemon->ability_id = get_ability(pokemon_id);

    pokemon->ev[HP] = 0;
    pokemon->ev[ATK] = 0;
    pokemon->ev[DEF] = 0;
    pokemon->ev[SP_ATK] = 0;
    pokemon->ev[SP_DEF] = 0;
    pokemon->ev[SPD] = 0;

    generate_moves(pokemon, level);

    pokemon->iv[HP] = 1 + rand() % 31;
    pokemon->iv[ATK] = 1 + rand() % 31;
    pokemon->iv[DEF] = 1 + rand() % 31;
    pokemon->iv[SP_ATK] = 1 + rand() % 31;
    pokemon->iv[SP_DEF] = 1 + rand() % 31;
    pokemon->iv[SPD] = 1 + rand() % 31;

    pokemon->nickname[0] = '\0';

    pokemon->level = level;
    pokemon->status = NO_STATUS;

    pokemon->egg = false;
    pokemon->cycles_to_hatch = 0;

    pokemon->nature = rand() % (SERIOUS+1);

    calculate_stats(pokemon);

    pokemon->current_hp = pokemon->stat[HP];
}

void print_pokemon(struct Pokemon *pokemon)
{
    printf("%s - Lv. %i\n", pokemons[pokemon->pokemon_id].name, pokemon->level);
    if (pokemon->level < 100)
	printf("\tExp: %i (%i to next level)\n", pokemon->experience, get_experience_at_level(pokemon->pokemon_id, pokemon->level+1) - pokemon->experience);
    else
	printf("\tExp: %i\n", pokemon->experience);
    printf("\tAbility: %s\n", abilities[pokemon->ability_id].name);
    if (pokemon->held_item_id == 0)
	printf("\tHeld Item: None\n");
    else
	printf("\tHeld Item: %s\n", items[pokemon->held_item_id].name);
    printf("\tNature: %s\n", get_nature_string(pokemon->nature));
    printf("\tStat: HP ATK DEF SP_ATK SP_DEF SPD\n");
    printf("\tIV:  %3i %3i %3i    %3i    %3i %3i\n", pokemon->iv[HP], pokemon->iv[ATK], pokemon->iv[DEF], pokemon->iv[SP_ATK], pokemon->iv[SP_DEF], pokemon->iv[SPD]);
    printf("\tEV:  %3i %3i %3i    %3i    %3i %3i\n", pokemon->ev[HP], pokemon->ev[ATK], pokemon->ev[DEF], pokemon->ev[SP_ATK], pokemon->ev[SP_DEF], pokemon->ev[SPD]);
    printf("\tTot: %3i %3i %3i    %3i    %3i %3i\n", pokemon->stat[HP], pokemon->stat[ATK], pokemon->stat[DEF], pokemon->stat[SP_ATK], pokemon->stat[SP_DEF], pokemon->stat[SPD]);
    printf("\tCurrent HP: %i / %i\n", pokemon->current_hp, pokemon->stat[HP]);

    printf("\tMoveset:\n");
    for (int i = 0; i < 4; ++i) {
	if (pokemon->move[i].id == 0)
	    continue;
	
	printf("\t\t%i. %s (%i/%i)\n", i+1, moves[pokemon->move[i].id].name, pokemon->move[i].pp,moves[pokemon->move[i].id].pp);
    }
}

static void calculate_stats(struct Pokemon *pokemon)
{
    int *base = pokemons[pokemon->pokemon_id].base_stats;
    int *iv = pokemon->iv;
    int *ev = pokemon->ev;
    int level = pokemon->level;
    int nature = pokemon->nature;

    pokemon->stat[HP] = (2 * base[HP] + iv[HP] + ev[HP] / 4) * level / 100 + level + 10;
    pokemon->stat[ATK] = ((2 * base[ATK] + iv[ATK] + ev[ATK] / 4) * level / 100 + 5) * get_nature_multiplier(nature, ATK);
    pokemon->stat[DEF] = ((2 * base[DEF] + iv[DEF] + ev[DEF] / 4) * level / 100 + 5) * get_nature_multiplier(nature, DEF);
    pokemon->stat[SP_ATK] = ((2 * base[SP_ATK] + iv[SP_ATK] + ev[SP_ATK] / 4) * level / 100 + 5) * get_nature_multiplier(nature, SP_ATK);
    pokemon->stat[SP_DEF] = ((2 * base[SP_DEF] + iv[SP_DEF] + ev[SP_DEF] / 4) * level / 100 + 5) * get_nature_multiplier(nature, SP_DEF);
    pokemon->stat[SPD] = ((2 * base[SPD] + iv[SPD] + ev[SPD] / 4) * level / 100 + 5) * get_nature_multiplier(nature, SPD);
}

char *nature_strings[] = {
    "Hardy", "Lonely", "Adamant", "Naughty", "Brave",
    "Bold", "Docile", "Impish", "Lax", "Relaxed",
    "Modest", "Mild", "Bashful", "Rash", "Quiet",
    "Calm", "Gentle", "Careful", "Quirky", "Sassy",
    "Timid", "Hasty", "Jolly", "Naive", "Serious"
};

char *get_nature_string(enum Nature nature)
{
    return nature_strings[nature];
}

static float get_nature_multiplier(enum Nature nature, int stat)
{
    if (stat == ATK) {
	if (nature == HARDY) return 1;
	else if (nature == BOLD) return 0.9;
	else if (nature == MODEST) return 0.9;
	else if (nature == CALM) return 0.9;
	else if (nature == TIMID) return 0.9;
	else if (nature == LONELY) return 1.1;
	else if (nature == ADAMANT) return 1.1;
	else if (nature == NAUGHTY) return 1.1;
	else if (nature == BRAVE) return 1.1;
    } else if (stat == DEF) {
	if (nature == DOCILE) return 1;
	else if (nature == LONELY) return 0.9;
	else if (nature == MILD) return 0.9;
	else if (nature == GENTLE) return 0.9;
	else if (nature == HASTY) return 0.9;
	else if (nature == BOLD) return 1.1;
	else if (nature == IMPISH) return 1.1;
	else if (nature == LAX) return 1.1;
	else if (nature == RELAXED) return 1.1;
    } else if (stat == SP_ATK) {
	if (nature == BASHFUL) return 1;
	else if (nature == ADAMANT) return 0.9;
	else if (nature == IMPISH) return 0.9;
	else if (nature == CAREFUL) return 0.9;
	else if (nature == JOLLY) return 0.9;
	else if (nature == MODEST) return 1.1;
	else if (nature == MILD) return 1.1;
	else if (nature == RASH) return 1.1;
	else if (nature == QUIET) return 1.1;
    } else if (stat == SP_DEF) {
	if (nature == QUIRKY) return 1;
	else if (nature == NAUGHTY) return 0.9;
	else if (nature == LAX) return 0.9;
	else if (nature == RASH) return 0.9;
	else if (nature == NAIVE) return 0.9;
	else if (nature == CALM) return 1.1;
	else if (nature == GENTLE) return 1.1;
	else if (nature == CAREFUL) return 1.1;
	else if (nature == SASSY) return 1.1;
    } else if (stat == SPD) {
	if (nature == SERIOUS) return 1;
	else if (nature == BRAVE) return 0.9;
	else if (nature == RELAXED) return 0.9;
	else if (nature == QUIET) return 0.9;
	else if (nature == SASSY) return 0.9;
	else if (nature == TIMID) return 1.1;
	else if (nature == HASTY) return 1.1;
	else if (nature == JOLLY) return 1.1;
	else if (nature == NAIVE) return 1.1;
    }

    return 1;
}

static void generate_moves(struct Pokemon *pokemon, int level)
{
    for (int i = 0; i < 4; ++i)
	    pokemon->move[i].id = 0;

    /* Iterate backwards from the level to 2, and populate the moveset. */
    for (int i = level; i >= 2; --i) {
	/* Look through the moves to find a match */
	for (int j = 0; j < 125; ++j) {
	    struct LearnableMove m = pokemons[pokemon->pokemon_id].learnset[j];

	    /* Not a level move */
	    if (m.level == -1 || m.name[0] == '\0')
		continue;

	    /* Not the right level */
	    if (m.level != i)
		continue;

	    /* If all slots are taken, we stop */
	    if (pokemon->move[3].id != 0)
		break;
	    
	    pokemon->move[3] = pokemon->move[2];
	    pokemon->move[2] = pokemon->move[1];
	    pokemon->move[1] = pokemon->move[0];
	    pokemon->move[0].id = get_move_by_name(m.name);

	    if (pokemon->move[3].id == -1) {
		printf("INVALID MOVE: %s\n", m.name);
		abort();
	    }
	}

	if (pokemon->move[3].id != 0)
	    break;
    }

    /* Add level 1 moves if possible, first count the number of level 1 moves */
    int c = 0;
    int ids[20] = {0};
    for (int j = 0; j < 125; ++j) {
	struct LearnableMove m = pokemons[pokemon->pokemon_id].learnset[j];

	/* Not a level move */
	if (m.level == -1 || m.name[0] == '\0')
	    continue;

	/* Not the right level */
	if (m.level != 1)
	    continue;
	
	ids[c] = get_move_by_name(m.name);
	
	if (ids[c] == -1) {
	    printf("INVALID MOVE: %s\n", m.name);
	    abort();
	}
	c++;
    }

    /* As long as there is space, choose a random move from the list and add it */
    int moves_left = c;
    while (pokemon->move[3].id == 0 && moves_left > 0) {
	int i = rand() % c;

	if (ids[i] == 0)
	    continue;
	
	pokemon->move[3] = pokemon->move[2];
	pokemon->move[2] = pokemon->move[1];
	pokemon->move[1] = pokemon->move[0];
	pokemon->move[0].id = ids[i];
	ids[i] = 0;
	moves_left--;
    }

    /* Add the pp information */
    for (int i = 0; i < 4; ++i) {
	if (pokemon->move[i].id == 0)
	    continue;
	pokemon->move[i].pp = moves[pokemon->move[i].id].pp;
	pokemon->move[i].pp_ups = 0;
    }

    return;
}

static int get_ability(int pokemon_id)
{
    int ability_id1 = get_ability_by_name(pokemons[pokemon_id].abilities[0]);
    int ability_id2 = get_ability_by_name(pokemons[pokemon_id].abilities[1]);

    if (ability_id2 == 0)
	return ability_id1;
    
    if (rand() % 2 == 0)
	return ability_id1;
    else
	return ability_id2;
}

static int get_experience_at_level(int pokemon_id, int level)
{
    int exp_rate = pokemons[pokemon_id].leveling_rate;

    if (level == 1)
	return 0;

    if (exp_rate == EXP_FLUCTUATING) {
	if (level <= 15)
	    return level * level * level * (((level + 1) / 3) + 24) / 50;
	else if (level <= 36)
	    return level * level * level * (level + 14) / 50;
	else
	    return level * level * level * (level / 2 + 32) / 50;
    } else if (exp_rate == EXP_SLOW) {
	return 5 * level * level * level / 4;
    } else if (exp_rate == EXP_MEDIUM_SLOW) {
	return 6 * level * level * level / 5 - 15 * level * level + 100 * level - 140;
    } else if (exp_rate == EXP_MEDIUM_FAST) {
	return level * level * level;
    } else if (exp_rate == EXP_FAST) {
	return 4 * level * level * level / 5;
    } else if (exp_rate == EXP_ERRATIC) {
	if (level <= 50)
	    return (level * level * level) * (100 - level) / 50;
	else if (level <= 68)
	    return (level * level * level) * (150 - level) / 100;
	else if (level <= 98)
	    return (level * level * level) * ((1911 - 10 * level) / 3) / 500;
	else
	    return (level * level * level) * (160 - level) / 100;
    }
}
