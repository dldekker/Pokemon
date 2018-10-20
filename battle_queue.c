#include "battle.h"

void queue_init(struct BattleQueue *queue)
{
	queue->top = NULL;
}

void queue_enqueue(struct BattleQueue *queue, struct Node node)
{
	/* Create a new node and fill it. */
	struct Node *new_node = malloc(sizeof(struct Node));
	*new_node = node;

	/* Add the node to the top, and link to the existing node list. */
	new_node->next = queue->top;
	queue->top = new_node;
}

int queue_get(struct BattleQueue *queue, struct Node *node)
{
	struct Node *tmp = queue->top;

	if (tmp == NULL)
		return -1;
	
	if (tmp->next == NULL) {
		*node = *tmp;
		return 0;
	}

	while (tmp->next->next != NULL)
		tmp = tmp->next;
	
	*node = *tmp->next;

	return 0;
}

void queue_dequeue(struct BattleQueue *queue)
{
	struct Node *tmp = queue->top;

	if (tmp == NULL)
		return;
	
	if (tmp->next == NULL) {
		queue->top = NULL;
		return;
	}

	while (tmp->next->next != NULL)
		tmp = tmp->next;
	
	tmp->next = NULL;

	return;
}

void queue_clear(struct BattleQueue *queue)
{
	// TODO: Clean up the queue properly.
	queue->top = NULL;
}
