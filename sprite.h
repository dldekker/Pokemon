#include <SDL2/SDL.h>
#include <GL/glew.h>

struct Sprite {
	GLuint vao;
	GLuint texture;
	GLuint program;
	
	int x, y, w, h;
	float a;
};

void sprite_init(struct Sprite *sprite, char *filename, int x, int y, int w, int h);
void sprite_render(struct Sprite *sprite, int w, int h);
void sprite_cleanup(struct Sprite *sprite);

void sprite_set_alpha(struct Sprite *sprite, float a);