#include "sprite.h"

#include <stdio.h>

#include "util.h"

void sprite_init(struct Sprite *sprite, char *filename, int x, int y, int w, int h)
{
	glCreateVertexArrays(1, &sprite->vao);
	sprite->texture = load_texture(filename);
	sprite->program = load_program("sprite.vs", 0, 0, 0, "sprite.fs");
	
	sprite->x = x;
	sprite->y = y;
	sprite->w = w;
	sprite->h = h;
	
	sprite->a = 100;
}

void sprite_render(struct Sprite *sprite, int w, int h)
{
	glBindVertexArray(sprite->vao);
	glBindTexture(GL_TEXTURE_2D, sprite->texture);
	glUseProgram(sprite->program);
	
	glUniform2f(0, w, h);
	glUniform4f(1, sprite->x, sprite->y, sprite->w, sprite->h);
	glUniform1f(2, sprite->a);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void sprite_cleanup(struct Sprite *sprite)
{
	
}

void sprite_set_alpha(struct Sprite *sprite, float a)
{
	if (a < 0)
		a = 0;
	else if (a > 100)
		a = 100;
	sprite->a = a;
}