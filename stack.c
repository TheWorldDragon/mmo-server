#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <string.h>
#include <stdbool.h>
#include "stack.h"

typedef struct stack_node node_t;

void stack_init(stack_t* this)
{
	bzero(this,sizeof(*this));
	this->head = calloc(1,sizeof(*this));
}
void stack_push(stack_t* this,void* element)
{
	this->element_number++;
	node_t* new_node = calloc(1,sizeof(*new_node));
	new_node->element = element;
	new_node->next = this->head->next;
	this->head->next = new_node;
};
void* pop(stack_t* this)
{
	this->element_number--;
	node_t* to_move = this->head->next;
	this->head->next = to_move->next;
	void* element = to_move->element;
	free(to_move);
	return element;
}
void* stack_pop(stack_t* this)
{
	if(this->element_number==0)
	{
		return NULL;
	}
	return pop(this);
}
void stack_destory(stack_t* this)
{
	while(this->element_number)
	{
		pop(this);
	}
	free(this->head);
	bzero(this,sizeof(*this));
}

