/*
 * author: dragon
 * description:a stack
 *
 */
#ifndef STACK_H
#define STACK_H

struct stack_node
{
	void* element;
	struct stack_node* next;
};
typedef struct
{
	struct stack_node* head;
	long element_number;
}stack_t;
void stack_init(stack_t* );
void stack_push(stack_t* ,void* element);
void* stack_pop(stack_t* );
void stack_destory(stack_t* );
#endif
