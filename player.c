#include "player.h"

#include <math.h>

#include "util.h"

#define SPEED 0.80

void static player_move(struct Player *player);
void static player_animation(struct Player *player);

void player_init(struct Player *player)
{
	glCreateVertexArrays(1, &player->vao);
	player->texture = load_texture("res/player.png");
	player->program = load_program("player.vs", 0, 0, 0, "player.fs");

	player->x = 32;
	player->y = 32;
	player->target_x = player->x;
	player->target_y = player->y;

	for (int i = 0; i < 6; ++i)
		player->party[i].pokemon_id = 0;

	player->dir = DOWN;
	player->mode = WALKING;

	for (int i = 0; i < 5; player->keys[i++] = false);
}

void player_event(struct Player *player, SDL_Event *e)
{
	if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_w) player->keys[0] = true;
		if (e->key.keysym.sym == SDLK_a) player->keys[1] = true;
		if (e->key.keysym.sym == SDLK_s) player->keys[2] = true;
		if (e->key.keysym.sym == SDLK_d) player->keys[3] = true;
		if (e->key.keysym.sym == SDLK_e) player->mode = 1 - player->mode;

		if (e->key.keysym.sym == SDLK_p)
			for (int i = 0; i < 6; ++i)
				if (player->party[i].pokemon_id != 0)
					print_pokemon(&player->party[i]);

		if (e->key.keysym.sym == SDLK_n)
			generate_pokemon(&player->party[0], 4, 5);

		if (e->key.keysym.sym == SDLK_b) player->keys[4] = true;
	}
	if (e->type == SDL_KEYUP) {
		if (e->key.keysym.sym == SDLK_w) player->keys[0] = false;
		if (e->key.keysym.sym == SDLK_a) player->keys[1] = false;
		if (e->key.keysym.sym == SDLK_s) player->keys[2] = false;
		if (e->key.keysym.sym == SDLK_d) player->keys[3] = false;
		if (e->key.keysym.sym == SDLK_b) player->keys[4] = false;
	}
}

void player_update(struct Player *player, bool *battle)
{
	if (player->keys[4])
		*battle = true;

	player_move(player);

	player_animation(player);
}

void static player_animation(struct Player *player)
{
	static int last_frame_moving = false;

	int aligned = false;
	if (fabs(player->target_y - player->y) < 0.0001)
		if (fabs(player->target_x - player->x) < 0.0001)
			aligned = true;
	
	bool moving = true;

	/* Look if we aren't moving. (Player is walking into rocks). */
	if (aligned) {
		if (player->keys[0] && !player->keys[2]) player->dir = UP;
		else if (player->keys[1] && !player->keys[3]) player->dir = LEFT;
		else if (player->keys[2] && !player->keys[0]) player->dir = DOWN;
		else if (player->keys[3] && !player->keys[1]) player->dir = RIGHT;
		else moving = false;
	} else {
		if (player->target_x > player->x) player->dir = RIGHT;
		if (player->target_x < player->x) player->dir = LEFT;
		if (player->target_y > player->y) player->dir = DOWN;
		if (player->target_y < player->y) player->dir = UP;
	}
	player->walking = moving;

	/* Start the animation */
	if (moving && !last_frame_moving)
		player->animation_start = SDL_GetTicks();
	
	if (moving) {
		int r = (SDL_GetTicks() - player->animation_start) % 800;

		if (r >= 0 && r <= 200) player->frame = 0;
		if (r >= 200 && r <= 400) player->frame = 1;
		if (r >= 400 && r <= 600) player->frame = 2;
		if (r >= 600 && r <= 800) player->frame = 3;
	} else {
		player->frame = 0;
	}


	last_frame_moving = moving;
}

void static player_move(struct Player *player)
{
	int aligned = false;
	if (fabs(player->target_y - player->y) < 0.0001)
		if (fabs(player->target_x - player->x) < 0.0001)
			aligned = true;

	if (aligned) {
		aligned = false;
		if (player->keys[0] && !player->keys[2]) player->target_y -= 16;
		else if (player->keys[1] && !player->keys[3]) player->target_x -= 16;
		else if (player->keys[2] && !player->keys[0]) player->target_y += 16;
		else if (player->keys[3] && !player->keys[1]) player->target_x += 16;
		else aligned = true;
	}
	if (!aligned) {
		float speed = SPEED;
		if (player->mode == BIKING)
			speed *= 2;

		if (player->target_x > player->x) {
			player->x += speed;
			if (player->x > player->target_x) {
				if (!player->keys[3])
					player->x = player->target_x;
				else
					player->target_x += 16;
			}
		}
		if (player->target_x < player->x) {
			player->x -= speed;
			if (player->x < player->target_x) {
				if (!player->keys[1])
					player->x = player->target_x;
				else
					player->target_x -= 16;
			}
		}
		if (player->target_y > player->y) {
			player->y += speed;
			if (player->y > player->target_y) {
				if (!player->keys[2])
					player->y = player->target_y;
				else
					player->target_y += 16;
			}
		}
		if (player->target_y < player->y) {
			player->y -= speed;
			if (player->y < player->target_y) {
				if (!player->keys[0])
					player->y = player->target_y;
				else
					player->target_y -= 16;
			}
		}
	}

}

void player_render(struct Player *player, int w, int h, int cx, int cy, float cz)
{
	glBindVertexArray(player->vao);
	glBindTexture(GL_TEXTURE_2D, player->texture);
	glUseProgram(player->program);

	glUniform2f(0, w, h);
	glUniform2i(3, cx, cy);
	glUniform1f(4, cz);

	// Provide the coordinates of the player sprite. Bit of a hack.
	if (player->mode == WALKING) {
		glUniform4f(5, player->x + 1, player->y - 6, 14, 21);
		if (player->dir == DOWN) {
			if (player->frame == 0) glUniform4f(6, 0,  0, 14, 21);
			if (player->frame == 1) glUniform4f(6, 0, 22, 14, 21);
			if (player->frame == 2) glUniform4f(6, 0,  0, 14, 21);
			if (player->frame == 3) glUniform4f(6, 0, 44, 14, 21);
		} else if (player->dir == UP) {
			if (player->frame == 0) glUniform4f(6,15,  0, 14, 21);
			if (player->frame == 1) glUniform4f(6,15, 22, 14, 21);
			if (player->frame == 2) glUniform4f(6,15,  0, 14, 21);
			if (player->frame == 3) glUniform4f(6,15, 44, 14, 21);
		} else if (player->dir == LEFT) {
			glUniform4f(5, player->x + 2, player->y - 6, 14, 21);
			if (player->frame == 0) glUniform4f(6,30,  0, 14, 21);
			if (player->frame == 1) glUniform4f(6,30, 22, 14, 21);
			if (player->frame == 2) glUniform4f(6,30,  0, 14, 21);
			if (player->frame == 3) glUniform4f(6,30, 44, 14, 21);
		} else if (player->dir == RIGHT) {
			glUniform4f(5, player->x + 0, player->y - 6, 14, 21);
			if (player->frame == 0) glUniform4f(6,45,  0, 14, 21);
			if (player->frame == 1) glUniform4f(6,45, 22, 14, 21);
			if (player->frame == 2) glUniform4f(6,45,  0, 14, 21);
			if (player->frame == 3) glUniform4f(6,45, 44, 14, 21);
		} 
	} else if (player->mode == BIKING) {
		glUniform4f(5, player->x + 1, player->y - 8, 15, 23);
		if (player->dir == DOWN) {
			if (player->frame == 0) {
				glUniform4f(6,74,  0, 15, 23);
			} else if (player->frame == 1) {
				glUniform4f(6,74, 24, 15, 23);
				glUniform4f(5, player->x, player->y - 8, 15, 23);
			} else if (player->frame == 2) {
				glUniform4f(6,74,  0, 15, 23);
			} else if (player->frame == 3) {
				glUniform4f(6,74, 48, 15, 23);
			}
		} else if (player->dir == UP) {
			if (player->frame == 0) {
				glUniform4f(6,89,  0, 15, 23);
			} else if (player->frame == 1) {
				glUniform4f(6,90, 24, 15, 23);
				glUniform4f(5, player->x, player->y - 8, 15, 23);
			} else if (player->frame == 2) {
				glUniform4f(6,89,  0, 15, 23);
			} else if (player->frame == 3) {
				glUniform4f(6,89, 48, 15, 23);
			}
		} else if (player->dir == LEFT) {
			glUniform4f(5, player->x - 4, player->y - 8, 22, 23);
			if (player->frame == 0) {
				glUniform4f(6, 105, 0, 22, 23);
			} else if (player->frame == 1) {
				glUniform4f(6, 105, 24, 22, 23);
			} else if (player->frame == 2) {
				glUniform4f(6, 105, 0, 22, 23);
			} else if (player->frame == 3) {
				glUniform4f(6, 106, 48, 22, 23);
			}
		} else if (player->dir == RIGHT) {
			glUniform4f(5, player->x - 2, player->y - 8, 22, 23);
			if (player->frame == 0) {
				glUniform4f(6, 129, 0, 22, 23);
			} else if (player->frame == 1) {
				glUniform4f(6, 129, 24, 22, 23);
			} else if (player->frame == 2) {
				glUniform4f(6, 129, 0, 22, 23);
			} else if (player->frame == 3) {
				glUniform4f(6, 128, 48, 22, 23);
			}
		} 
		
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void player_cleanup(struct Player *player)
{

}
