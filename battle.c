#include "battle.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void battle_init(struct Battle *battle, struct Player *player)
{
	battle->player = player;

	queue_init(&battle->queue);

	struct Node idle;
	idle.type = IDLE;
	idle.data.idle.wait_until = SDL_GetTicks() + 1000;
	queue_enqueue(&battle->queue, idle);
}

void battle_start(struct Battle *battle)
{
}

void battle_update(struct Battle *battle)
{
	struct Node action;

	if (queue_get(&battle->queue, &action) < 0) {
		printf("ERROR: QUEUE EMPTY\n");
		abort();
	}

	// If we are idle, just repush the idle action.
	if (action.type == IDLE) {
		/* The time has been reached. */
		if (action.data.idle.wait_until < SDL_GetTicks()) {
			queue_dequeue(&battle->queue);

			int ret = queue_get(&battle->queue, &action);
			if (action.type != IDLE || ret < 0) {
				printf(".\n");
				struct Node idle;
				idle.type = IDLE;
				idle.data.idle.wait_until = SDL_GetTicks() + 1000;
				queue_enqueue(&battle->queue, idle);
			}
		}
	} else if (action.type == MESSAGE) {
		queue_dequeue(&battle->queue);
		printf("%s", action.data.message.message);

		struct Node idle;
		idle.type = IDLE;
		idle.data.idle.wait_until = SDL_GetTicks() + 1000;
		queue_enqueue(&battle->queue, idle);
	}
}

void battle_render(struct Battle *battle)
{

}

void battle_event(struct Battle *battle, SDL_Event *e)
{
	if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_p) {
			struct Node m;
			m.type = MESSAGE;
			strcpy(m.data.message.message, "Message!\n");
			queue_enqueue(&battle->queue, m);
		}
	}
}

void battle_destroy(struct Battle *battle)
{

}
