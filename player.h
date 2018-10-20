#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "pokemon.h"

enum Direction {LEFT, RIGHT, UP, DOWN};
enum Mode {WALKING, BIKING};

struct Player {
	SDL_Window *window;
	GLuint vao;
	GLuint texture;
	GLuint program;

	float x;
	float y;

	int target_x;
	int target_y;

	enum Direction dir;
	enum Mode mode;
	int animation_start;
	int frame;
	int walking;

	struct Pokemon party[6];

	bool keys[5];

	GLuint buffer;
};

void player_init(struct Player *player);
void player_event(struct Player *player, SDL_Event *e);
void player_update(struct Player *player, bool *battle);
void player_render(struct Player *player, int w, int h, int cx, int cy, float cz);
void player_cleanup(struct Player *player);

#endif
