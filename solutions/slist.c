#include <stdio.h>
#include <stdlib.h>
#include "slist.h"

SNode* add_node(SNode *head, int value)	{
	SNode *node = malloc(sizeof(SNode));

	if (!node) return head;

		node->value = value;
		node->next = head;

	return node;
}

int count_nodes(SNode *head)	{

	int count = 0;
	for (SNode *p = head; p != NULL; p = p->next)
		count++;

	return count;
}

void free_list(SNode *head)	{
	SNode *tmp;

	while (head)	{
		tmp = head;
		head = head->next;
	free(tmp);
	}
}
