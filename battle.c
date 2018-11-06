#include "battle.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void action_idle(struct Battle *battle, struct Node *action);
void action_message(struct Battle *battle, struct Node *action);
void action_screen_reveal(struct Battle *battle, struct Node *action);

void render_idle(struct Battle *battle, struct Node *action, int w, int h);
void render_message(struct Battle *battle, struct Node *action, int w, int h);
void render_screen_reveal(struct Battle *battle, struct Node *action, int w, int h);

void enqueue_idle(struct BattleQueue *queue);
void enqueue_message(struct Battle *battle, struct BattleQueue *queue, char *str);
void enqueue_screen_reveal(struct Battle *battle, struct BattleQueue *queue, float fade_time);

void battle_init(struct Battle *battle, struct Player *player)
{
	battle->player = player;

	queue_init(&battle->queue);
	
	sprite_init(&battle->s, "res/battle_background.png", 0, 0, 960, 640);
	battletext_init(&battle->battletext, 40, 480, 880, 140);
	
	battle->key = false;
}

void battle_start(struct Battle *battle)
{
	/* Push the initial actions on the queue */
	enqueue_screen_reveal(battle, &battle->queue, 0.3);
	enqueue_message(battle, &battle->queue, "TRAINER RICK is very ready to battle!!!");
	enqueue_message(battle, &battle->queue, "TRAINER RICK used BULBASAUR.");
	enqueue_message(battle, &battle->queue, "CHARMANDER, I choose you!");
	enqueue_idle(&battle->queue);
}

void battle_update(struct Battle *battle)
{
	struct Node *action, *tmp;
	enum NodeType old_type;

	if (queue_get(&battle->queue, &action) < 0) {
		fprintf(stderr, "ERROR: QUEUE EMPTY\n");
		abort();
	}
	
	old_type = action->type;
	
	// If we are idle, just repush the idle action.
	if (action->type == IDLE) {
		action_idle(battle, action);
	} else if (action->type == MESSAGE) {
		action_message(battle, action);
	} else if (action->type == SCREEN_REVEAL) {
		action_screen_reveal(battle, action);
	}
}

void battle_render(struct Battle *battle, int w, int h)
{
	struct Node *action;

	if (queue_get(&battle->queue, &action) < 0) {
		fprintf(stderr, "ERROR: QUEUE EMPTY\n");
		abort();
	}

	// If we are idle, just repush the idle action.
	if (action->type == IDLE) {
		render_idle(battle, action, w, h);
	} else if (action->type == MESSAGE) {
		render_message(battle, action, w, h);
	} else if (action->type == SCREEN_REVEAL) {
		render_screen_reveal(battle, action, w, h);
	}
}

void battle_event(struct Battle *battle, SDL_Event *e)
{
	if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_p) {
			enqueue_message(battle, &battle->queue, "DEBUG1");
			enqueue_message(battle, &battle->queue, "DEBUG2");
			enqueue_message(battle, &battle->queue, "DEBUG3");
			enqueue_message(battle, &battle->queue, "DEBUG4");
			enqueue_idle(&battle->queue);
		} else if (e->key.keysym.sym == SDLK_SPACE) {
			battle->key = true;
		}
	} if (e->type == SDL_KEYUP) {
		if (e->key.keysym.sym == SDLK_SPACE) {
			battle->key = false;
		}
	}
}

void battle_destroy(struct Battle *battle)
{

}

/* HANDLE BATTLE QUEUE ACTION RENDERS (these must *not* change any state */
void render_idle(struct Battle *battle, struct Node *action, int w, int h)
{
	/* Render the background */
	sprite_render(&battle->s, w, h);
	
	/* Render text */
	battletext_render(&battle->battletext, w, h);
}
void render_message(struct Battle *battle, struct Node *action, int w, int h)
{
	/* Render the background */
	sprite_render(&battle->s, w, h);
	
	battletext_render(&battle->battletext, w, h);
}
void render_screen_reveal(struct Battle *battle, struct Node *action, int w, int h)
{
	/* Render the background */
	sprite_render(&battle->s, w, h);
	
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
}
void action_message(struct Battle *battle, struct Node *action)
{
	if (action->data.message.cur > action->data.message.length) {
		printf("%i\n", action->data.message.keystate);
		if (!battle->key && action->data.message.keystate == 0)
			action->data.message.keystate = 1;
		else if (battle->key && action->data.message.keystate == 1)
			action->data.message.keystate = 2;
		else if (!battle->key && action->data.message.keystate == 2)
			queue_dequeue(&battle->queue);
	} else {
		long delta = SDL_GetTicks() - action->data.message.start_time;
		int value = 100;
		if (battle->key == true)
			value = 10;
		if (delta > value) {
			action->data.message.start_time = SDL_GetTicks();
			
			char buffer[100] = {0};
			memcpy(buffer, action->data.message.buffer, action->data.message.cur);
			battletext_set_text2(&battle->battletext, buffer);
			
			action->data.message.cur += 1;
		}
	}
}
void action_screen_reveal(struct Battle *battle, struct Node *action)
{
	/* Only start the clock when the action has begun */
	if (action->data.screen_reveal.start_time == -1)
		action->data.screen_reveal.start_time = SDL_GetTicks();
	
	long delta = SDL_GetTicks() - action->data.screen_reveal.start_time;
	sprite_set_alpha(&battle->s, (float)delta / action->data.screen_reveal.fade_time);
	
	if (delta > action->data.screen_reveal.fade_time)
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
void enqueue_message(struct Battle *battle, struct BattleQueue *queue, char *str)
{
	struct Node n;
	n.type = MESSAGE;
	n.data.message.length = strlen(str);
	n.data.message.start_time = SDL_GetTicks();
	n.data.message.cur = 0;
	n.data.message.keystate = 0;
	strcpy(n.data.message.buffer, str);
	queue_enqueue(queue, n);
}
void enqueue_screen_reveal(struct Battle *battle, struct BattleQueue *queue, float fade_time)
{
	struct Node n;
	n.type = SCREEN_REVEAL;
	n.data.screen_reveal.start_time = -1;
	n.data.screen_reveal.fade_time = fade_time * 1000;
	sprite_set_alpha(&battle->s, 0);
	queue_enqueue(queue, n);
}