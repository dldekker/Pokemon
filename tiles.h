#ifndef TILES_H
#define TILES_C

#include <SDL2/SDL.h>
#include <GL/glew.h>

#define TILE_SIZE 16

struct Tiles {
	SDL_Window *window;
	GLuint vao;
	GLuint tileset_texture;
	GLuint program;

	int tiles_x;
	int tiles_y;
	int *tiles;
	GLuint pos_buffer;
	GLuint uv_buffer;
	GLuint tile_buffer;
};

void tiles_init(struct Tiles *tiles, SDL_Window *window, int tiles_x, int tiles_y);
void tiles_update(struct Tiles *tiles);
void tiles_render(struct Tiles *tiles, int cx, int cy, float cz);
void tiles_cleanup(struct Tiles *tiles);

#endif
