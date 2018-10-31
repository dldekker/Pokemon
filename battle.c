#include "battle.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void action_idle(struct Battle *battle, struct Node *action);
void action_debug_message(struct Battle *battle, struct Node *action);
void action_screen_reveal(struct Battle *battle, struct Node *action);

void enqueue_idle(struct BattleQueue *queue);
void enqueue_debug_message(struct BattleQueue *queue, char *str);
void enqueue_screen_reveal(struct BattleQueue *queue);

void battle_init(struct Battle *battle, struct Player *player)
{
	battle->player = player;

	queue_init(&battle->queue);
}

void battle_start(struct Battle *battle)
{
	/* Push the initial actions on the queue */
	enqueue_screen_reveal(&battle->queue);
	enqueue_debug_message(&battle->queue, "TRAINER JOEY is ready to battle!\n");
	enqueue_idle(&battle->queue);
}

void battle_update(struct Battle *battle)
{
	struct Node *action;

	if (queue_get(&battle->queue, &action) < 0) {
		fprintf(stderr, "ERROR: QUEUE EMPTY\n");
		abort();
	}

	// If we are idle, just repush the idle action.
	if (action->type == IDLE) {
		action_idle(battle, action);
	} else if (action->type == DEBUG_MESSAGE) {
		action_debug_message(battle, action);
	} else if (action->type == SCREEN_REVEAL) {
		action_screen_reveal(battle, action);
	}
}

void battle_render(struct Battle *battle)
{
	
}

void battle_event(struct Battle *battle, SDL_Event *e)
{
	if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_p) {
			enqueue_debug_message(&battle->queue, "DEBUG1");
			enqueue_debug_message(&battle->queue, "DEBUG2");
			enqueue_debug_message(&battle->queue, "DEBUG3");
			enqueue_debug_message(&battle->queue, "DEBUG4");
			enqueue_idle(&battle->queue);
		}
	}
}

void battle_destroy(struct Battle *battle)
{

}

/* HANDLE BATTLE QUEUE ACTION RENDERS (these must *not* change any state */
void action_screen_reveal(struct Battle *battle)
{
	/* Rnder the background */
	
	
	/* Render the black eclipse above it */
	
}

/* HANDLE BATTLE QUEUE ACTION UPDATES (these should update all state) */
void action_idle(struct Battle *battle, struct Node *action)
{
	/* The time has been reached. */
	if (action->data.idle.wait_until < SDL_GetTicks()) {
		queue_dequeue(&battle->queue);

		/* If there are no more nodes, insert an idle node */
		int ret = queue_get(&battle->queue, &action);
		if (action->type != IDLE || ret < 0) {
			enqueue_idle(&battle->queue);
		}
	}
	printf(".");
}
void action_debug_message(struct Battle *battle, struct Node *action)
{
	long delta = SDL_GetTicks() - action->data.debug_message.start_time;
	if (delta > 100) {
		action->data.debug_message.start_time = SDL_GetTicks();
		printf("%c", action->data.debug_message.message[action->data.debug_message.cur]);
		action->data.debug_message.cur += 1;
		if (action->data.debug_message.cur > strlen(action->data.debug_message.message)) {
			queue_dequeue(&battle->queue);
		}
	}
}
void action_screen_reveal(struct Battle *battle, struct Node *action)
{
	/* Only start the clock when the action has begun */
	if (action->data.screen_reveal.start_time == -1)
		action->data.screen_reveal.start_time = SDL_GetTicks();
	
	long delta = SDL_GetTicks() - action->data.screen_reveal.start_time;
	printf("TIME: %i\n", delta);
	
	if (delta > 5000)
		queue_dequeue(&battle->queue);
}

/* HELPERS TO PLACE ACTIONS ON THE QUEUE */
void enqueue_idle(struct BattleQueue *queue)
{
	struct Node n;
	n.type = IDLE;
	n.data.idle.wait_until = SDL_GetTicks() + 1000;
	queue_enqueue(queue, n);
}
void enqueue_debug_message(struct BattleQueue *queue, char *str)
{
	struct Node n;
	n.type = DEBUG_MESSAGE;
	n.data.debug_message.start_time = SDL_GetTicks();
	n.data.debug_message.cur = 0;
	strcpy(n.data.debug_message.message, str);
	queue_enqueue(queue, n);
}
void enqueue_screen_reveal(struct BattleQueue *queue)
{
	struct Node n;
	n.type = SCREEN_REVEAL;
	n.data.screen_reveal.start_time = -1;
	queue_enqueue(queue, n);
}