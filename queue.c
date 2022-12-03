#include "std.h"
#include "queue.h"
typedef queue_node_t node_t;
void queue_init(queue_t* this)
{
	bzero(this,sizeof(*this));
	this->head=calloc(1,sizeof(*this->head));
	this->tail=this->head;
}
void queue_inqueue(queue_t* this,void*element)
{
	node_t* new_node = calloc(1,sizeof(*new_node));
	new_node->element=element;
	this->tail->next=new_node;
	this->tail=this->tail->next;
	this->element_number++;
}
void* queue_dequeue(queue_t* this)
{
	if(this->element_number==0)
		return NULL;
	node_t* return_node = this->head->next;
	this->head->next=this->head->next->next;
	this->element_number--;
	void* return_element = return_node->element;
	free(return_node);
	if(this->element_number==0)
	{
		this->tail=this->head;
	}
	return return_element;
}
void queue_destory(queue_t* this)
{
	while(this->element_number!=0)
	{
		queue_dequeue(this);
	}
	free(this->head);
	bzero(this,sizeof(*this));
}

