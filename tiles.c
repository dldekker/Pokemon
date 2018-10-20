#include "tiles.h"

#include "util.h"

void tiles_init(struct Tiles *tiles, SDL_Window *window, int tiles_x, int tiles_y)
{
	tiles->window = window;
	tiles->tiles_x = tiles_x;
	tiles->tiles_y = tiles_y;

	glCreateVertexArrays(1, &tiles->vao);
	tiles->tileset_texture = load_texture("tiles.png");
	tiles->program = load_program("tiles.vs", 0, 0, 0, "tiles.fs");

	/* Generate the tileset */
	tiles->tiles = calloc(tiles_x * tiles_y, sizeof(int));

	/* Generate the vertex information used for rendering */
	int k = 0;
	int *tmp = malloc(sizeof(int) * 5 * tiles_x * tiles_y * 6);
	for (int i = 0; i < tiles_y; ++i) {
		for (int j = 0; j < tiles_x; ++j) {
			int base = (i*tiles_x+j)*30;
			int r = rand() % 2;
			if (r == 1) r = 88 + 5;
			if (r == 0) r = 1;
			tmp[base+ 0] = j * TILE_SIZE;
			tmp[base+ 1] = i * TILE_SIZE;
			tmp[base+ 2] = 0;
			tmp[base+ 3] = 0;
			tmp[base+ 4] = r;
			tmp[base+ 5] = j * TILE_SIZE;
			tmp[base+ 6] = (i+1) * TILE_SIZE;
			tmp[base+ 7] = 0;
			tmp[base+ 8] = 1;
			tmp[base+ 9] = r;
			tmp[base+10] = (j+1) * TILE_SIZE;
			tmp[base+11] = (i+1) * TILE_SIZE;
			tmp[base+12] = 1;
			tmp[base+13] = 1;
			tmp[base+14] = r;
			tmp[base+15] = j * TILE_SIZE;
			tmp[base+16] = i * TILE_SIZE;
			tmp[base+17] = 0;
			tmp[base+18] = 0;
			tmp[base+19] = r;
			tmp[base+20] = (j+1) * TILE_SIZE;
			tmp[base+21] = i * TILE_SIZE;
			tmp[base+22] = 1;
			tmp[base+23] = 0;
			tmp[base+24] = r;
			tmp[base+25] = (j+1) * TILE_SIZE;
			tmp[base+26] = (i+1) * TILE_SIZE;
			tmp[base+27] = 1;
			tmp[base+28] = 1;
			tmp[base+29] = r;
		}
	}
	glCreateBuffers(1, &tiles->vertex_buffer);
	glNamedBufferStorage(tiles->vertex_buffer,
			sizeof(int) * 5 * tiles_x * tiles_y * 6,
			tmp,
			0);
	glVertexArrayVertexBuffer(tiles->vao, 0, tiles->vertex_buffer, 0, sizeof(int) * 5);
	glVertexArrayAttribBinding(tiles->vao, 0, 0);
	glVertexArrayAttribBinding(tiles->vao, 1, 0);
	glVertexArrayAttribBinding(tiles->vao, 2, 0);
	glVertexArrayAttribIFormat(tiles->vao, 0, 2, GL_INT, 0); //POS (2 INT)
	glVertexArrayAttribIFormat(tiles->vao, 1, 2, GL_INT, 8); //UV (2 INT)
	glVertexArrayAttribIFormat(tiles->vao, 2, 1, GL_INT, 16); //TILE (1 INT)
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
