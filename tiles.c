#include "tiles.h"

#include <stdlib.h>

#include "util.h"

void tiles_init(struct Tiles *tiles, SDL_Window *window, int tiles_x, int tiles_y)
{
	tiles->window = window;
	tiles->tiles_x = tiles_x;
	tiles->tiles_y = tiles_y;

	glCreateVertexArrays(1, &tiles->vao);
	tiles->tileset_texture = load_texture("res/tiles.png");
	tiles->program = load_program("tiles.vs", 0, 0, 0, "tiles.fs");

	/* Generate the tileset */
	tiles->tiles = calloc(tiles_x * tiles_y, sizeof(int));

	/* Generate the vertex information used for rendering */
	int k = 0;
	int *tmp0 = malloc(sizeof(int) * 2 * tiles_x * tiles_y * 6);
	int *tmp1 = malloc(sizeof(int) * 2 * tiles_x * tiles_y * 6);
	int *tmp2 = malloc(sizeof(int) * 1 * tiles_x * tiles_y * 6);
	for (int i = 0; i < tiles_y; ++i) {
		for (int j = 0; j < tiles_x; ++j) {
			int base = (i*tiles_x+j);
			int r = rand() % 2;
			if (r == 1) r = 88+5;
			if (r == 0) r = 1;
			tmp0[base*12+ 0] = j * TILE_SIZE;
			tmp0[base*12+ 1] = i * TILE_SIZE;
			tmp0[base*12+ 2] = j * TILE_SIZE;
			tmp0[base*12+ 3] = (i+1) * TILE_SIZE;
			tmp0[base*12+ 4] = (j+1) * TILE_SIZE;
			tmp0[base*12+ 5] = (i+1) * TILE_SIZE;
			tmp0[base*12+ 6] = j * TILE_SIZE;
			tmp0[base*12+ 7] = i * TILE_SIZE;
			tmp0[base*12+ 8] = (j+1) * TILE_SIZE;
			tmp0[base*12+ 9] = i * TILE_SIZE;
			tmp0[base*12+10] = (j+1) * TILE_SIZE;
			tmp0[base*12+11] = (i+1) * TILE_SIZE;
			tmp1[base*12+ 0] = 0;
			tmp1[base*12+ 1] = 0;
			tmp1[base*12+ 2] = 0;
			tmp1[base*12+ 3] = 1;
			tmp1[base*12+ 4] = 1;
			tmp1[base*12+ 5] = 1;
			tmp1[base*12+ 6] = 0;
			tmp1[base*12+ 7] = 0;
			tmp1[base*12+ 8] = 1;
			tmp1[base*12+ 9] = 0;
			tmp1[base*12+10] = 1;
			tmp1[base*12+11] = 1;
			tmp2[base*6+ 0] = r;
			tmp2[base*6+ 1] = r;
			tmp2[base*6+ 2] = r;
			tmp2[base*6+ 3] = r;
			tmp2[base*6+ 4] = r;
			tmp2[base*6+ 5] = r;
		}
	}
	glCreateBuffers(1, &tiles->pos_buffer);
	glCreateBuffers(1, &tiles->uv_buffer);
	glCreateBuffers(1, &tiles->tile_buffer);
	
	glNamedBufferStorage(tiles->pos_buffer,
			sizeof(int) * 2 * tiles_x * tiles_y * 6,
			tmp0,
			0);
	glNamedBufferStorage(tiles->uv_buffer,
			sizeof(int) * 2 * tiles_x * tiles_y * 6,
			tmp1,
			0);
	glNamedBufferStorage(tiles->tile_buffer,
			sizeof(int) * 1 * tiles_x * tiles_y * 6,
			tmp2,
			0);
	glVertexArrayVertexBuffer(tiles->vao, 0, tiles->pos_buffer, 0, sizeof(int) * 2);
	glVertexArrayVertexBuffer(tiles->vao, 1, tiles->uv_buffer, 0, sizeof(int) * 2);
	glVertexArrayVertexBuffer(tiles->vao, 2, tiles->tile_buffer, 0, sizeof(int) * 1);
	
	glVertexArrayAttribBinding(tiles->vao, 0, 0);
	glVertexArrayAttribBinding(tiles->vao, 1, 1);
	glVertexArrayAttribBinding(tiles->vao, 2, 2);
	glVertexArrayAttribIFormat(tiles->vao, 0, 2, GL_INT, 0); //POS (2 INT)
	glVertexArrayAttribIFormat(tiles->vao, 1, 2, GL_INT, 0); //UV (2 INT)
	glVertexArrayAttribIFormat(tiles->vao, 2, 1, GL_INT, 0); //TILE (1 INT)
	glEnableVertexArrayAttrib(tiles->vao, 0);
	glEnableVertexArrayAttrib(tiles->vao, 1);
	glEnableVertexArrayAttrib(tiles->vao, 2);
}

void tiles_update(struct Tiles *tiles)
{

}

void tiles_render(struct Tiles *tiles, int cx, int cy, float cz)
{
	glBindVertexArray(tiles->vao);
	glBindTexture(GL_TEXTURE_2D, tiles->tileset_texture);
	glUseProgram(tiles->program);

	int x, y;
	SDL_GetWindowSize(tiles->window, &x, &y);
	glViewport(0, 0, x, y);

	glUniform2f(0, x, y);
	glUniform1i(1, 88);
	glUniform1i(2, 69);
	glUniform2i(3, cx, cy);
	glUniform1f(4, cz);
	glDrawArrays(GL_TRIANGLES, 0, tiles->tiles_x * tiles->tiles_y * 6);
}

void tiles_cleanup(struct Tiles *tiles)
{

}
