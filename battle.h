#ifndef BATTLE_H
#define BATTLE_H

#include "player.h"
#include "sprite.h"
#include "battletext.h"

enum NodeType {
	IDLE,
	MESSAGE,
	SCREEN_REVEAL
};

struct Node {
	enum NodeType type;
	union {
		struct {
			int keystate;
			char buffer[100];
			int length;
			long start_time;
			int cur;
		} message;
		
		struct {
			int wait_until;
		} idle;
		
		struct {
			long start_time;
			long fade_time;
		} screen_reveal;
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
	
	struct Sprite s;
	struct BattleText battletext;
	
	bool new_action;
	
	bool key;
};

void battle_init(struct Battle *battle, struct Player *player);
void battle_start(struct Battle *battle);
void battle_update(struct Battle *battle);
void battle_render(struct Battle *battle, int w, int h);
void battle_event(struct Battle *battle, SDL_Event *e);
void battle_destroy(struct Battle *battle);

void queue_init(struct BattleQueue *queue);
void queue_enqueue(struct BattleQueue *queue, struct Node node);
int queue_get(struct BattleQueue *queue, struct Node **node);
void queue_dequeue(struct BattleQueue *queue);
void queue_clear(struct BattleQueue *queue);

#endif
