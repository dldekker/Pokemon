#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "player.h"
#include "tiles.h"
#include "util.h"
#include "pokemon.h"
#include "battle.h"

struct State {
	SDL_Window *window;
	bool quit;

	int cam_x;
	int cam_y;
	float cam_zoom;

	struct Tiles tiles;
	struct Player player;
	struct Battle battle;
};

void render(struct State *state);
void update(struct State *state);
void events(struct State *state);

int main(int argc, char *argv[])
{
	struct State state = {0};
	
	srand(time(NULL));

	state.window = SDL_CreateWindow("3d", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(state.window);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	state.cam_x = 0;
	state.cam_y = 0;
	state.cam_zoom = 4;

	tiles_init(&state.tiles, state.window, 100, 100);
	player_init(&state.player);
	battle_init(&state.battle, &state.player);

	while (state.quit == false) {
		events(&state);

		update(&state);

		render(&state);

		SDL_GL_SwapWindow(state.window);
		
		 // Flush, so any printfs are actually printed (mingw hack).
		fflush(stdout);
	}

	return EXIT_SUCCESS;
}

bool battle = false;
void update(struct State *state)
{
	if (!battle) {
		tiles_update(&state->tiles);
		player_update(&state->player, &battle);
	} else {
		battle_update(&state->battle);
	}
}

void render(struct State *state)
{
	GLfloat color[] = {0, 0, 0, 1};
	glClearBufferfv(GL_COLOR, 0, color);

	int x, y;
	SDL_GetWindowSize(state->window, &x, &y);
	glViewport(0, 0, x, y);

	if (!battle) {
		tiles_render(&state->tiles, state->cam_x, state->cam_y, state->cam_zoom);
		player_render(&state->player, x, y, state->cam_x, state->cam_y, state->cam_zoom);
	} else {
		battle_render(&state->battle);
	}
}

void events(struct State *state)
{
	if (battle) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				state->quit = true;
			} else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) {
				state->quit = true;
			} else {
				battle_event(&state->battle, &e);
			}
		}

		return;
	}

	static int original_x = -1;
	static int original_y = -1;
	static int original_cam_x = -1;
	static int original_cam_y = -1;
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			state->quit = true;
		} else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) {
			state->quit = true;
		} else if (e.type == SDL_MOUSEBUTTONDOWN
				&& e.button.button == SDL_BUTTON_RIGHT) {
			if (original_x == -1) {
				original_x = e.button.x;
				original_y = e.button.y;
				original_cam_x = state->cam_x;
				original_cam_y = state->cam_y;
			}
		} else if (e.type == SDL_MOUSEMOTION) {
			int dx = e.motion.x - original_x;
			int dy = e.motion.y - original_y;
			if (original_x != -1) {
				state->cam_x = original_cam_x + (dx / state->cam_zoom);
				state->cam_y = original_cam_y + (dy / state->cam_zoom);
			}
		} else if (e.type == SDL_MOUSEBUTTONUP
				&& e.button.button == SDL_BUTTON_RIGHT) {
			original_x = -1;
			original_y = -1;
		} else if (e.type == SDL_MOUSEWHEEL) {
			if (e.wheel.y > 0) {
				state->cam_zoom *= 2;
			} else if (e.wheel.y < 0) {
				state->cam_zoom /= 2;
			}
		}

		player_event(&state->player, &e);
	}
}
