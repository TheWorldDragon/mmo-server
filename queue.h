/*
 * @author: dragon
 * @time: Nov. 6 17:00 2022
 * @description: A simple queue realization.
 */
#ifndef QUEUE_H
#define QUEUE_H
#include "std.h"
typedef struct queue_node
{
	void* element;
	struct queue_node* next;
}queue_node_t;
typedef struct
{
	queue_node_t* head;
	queue_node_t* tail;
	u_int64_t element_number;
}queue_t;
void queue_init(queue_t* );
void queue_inqueue(queue_t* ,void*element);
void* queue_dequeue(queue_t* );
void queue_destory(queue_t* );

#endif
