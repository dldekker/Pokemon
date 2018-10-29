#ifndef BATTLE_H
#define BATTLE_H

#include "player.h"

enum NodeType {IDLE, DEBUG_MESSAGE};

struct Node {
	enum NodeType type;
	union {
		struct {
			char message[100];
			long start_time;
			int cur;
		} debug_message;
		struct {
			int wait_until;
		} idle;
	} data;

	struct Node *next;
};

struct BattleQueue {
	struct Node *top;
};

/* Contains all the information of the battle */
struct Battle {
	struct Player *player;

	struct BattleQueue queue;
};

void battle_init(struct Battle *battle, struct Player *player);
void battle_start(struct Battle *battle);
void battle_update(struct Battle *battle);
void battle_render(struct Battle *battle);
void battle_event(struct Battle *battle, SDL_Event *e);
void battle_destroy(struct Battle *battle);

void queue_init(struct BattleQueue *queue);
void queue_enqueue(struct BattleQueue *queue, struct Node node);
int queue_get(struct BattleQueue *queue, struct Node **node);
void queue_dequeue(struct BattleQueue *queue);
void queue_clear(struct BattleQueue *queue);

#endif
