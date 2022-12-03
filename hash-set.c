#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hash-set.h"
#define DEFAULT_CAPACITY 10
typedef struct hash_set_node node_t;
static void free_all_node_with_element(hash_set_t* this);
static void normal_insert(hash_set_t* this,void* element);
static void all_nodes(hash_set_t* this, node_t** nodes);
static void all_elements0(hash_set_t* this, void** elements)
{
	node_t* nodes[this->number_of_member];
	all_nodes(this,nodes);
	for(int i=0;i<this->number_of_member;i++)
	{
		elements[i] = nodes[i]->element;
	}
}
static void all_nodes(hash_set_t* this, node_t** nodes)
{
	long founded_number = 0;
	for(long i=0;i<this->members_size;i++)
	{
		struct hash_set_node* head = this->members[i];
		for(struct hash_set_node* current = head;current->next!=NULL;current=current->next)
		{
			nodes[founded_number++] = current->next;
		}
	}
}
static void rehash(hash_set_t* this)
{
	void** all_element = malloc(sizeof(void*)*this->number_of_member);
	long number_of_member = this->number_of_member;
	all_elements0(this,all_element);
	free_all_node_with_element(this);
	this->members  = realloc(this->members,this->members_size*2*sizeof(void*));
	for(long i=this->members_size;i<this->members_size*2;i++)
	{
		this->members[i] = calloc(1,sizeof(struct hash_set_node));
	}
	this->members_size*=2;
	for(long i=0;i<number_of_member;i++)
	{
		normal_insert(this,all_element[i]);
	}
	free(all_element);
}
static void free_node(struct hash_set_node* head)
{
	if(head->next!=NULL)
	{
		free_node(head->next);
	}
	free(head);
}
static void free_all_node_with_element(hash_set_t* this)
{
	for(long i=0;i<this->members_size;i++)
	{
		struct hash_set_node* head = this->members[i];
		if(head->next!=NULL)
		{
			free_node(head->next);
			head->next=NULL;
		}
	}
	this->number_of_member=0;
}
static void normal_insert(hash_set_t* this,void* element)
{
	this->number_of_member ++;
	struct hash_set_node* new_node = calloc(1,sizeof(*new_node));
	new_node->element = element;
	long position = (long)element % this->members_size;
	struct hash_set_node* head = this->members[position];
	new_node->next = head->next;
	head->next =  new_node;
}
hash_set_t* all_elements(hash_set_t* this,void* elements)
{
	all_elements0(this,elements);
	return this;
}
hash_set_t* insert(hash_set_t* self,void* element)
{
	if(self->number_of_member > self->members_size/2)
	{
		rehash(self);
	}
	normal_insert(self,element);
	return self;
}
bool remove1(hash_set_t* this,void* element)
{
	long position = (long)element %this->members_size;
	struct hash_set_node* head = this->members[position];
	for(struct hash_set_node* current = head;current->next!=NULL;current=current->next)
	{
		if(current->next->element == element)
		{
			struct hash_set_node* tmp = current->next;
			current->next = tmp->next;
			free(tmp);
			this->number_of_member--;
			return true;
		}
	}
	return false;
}
hash_set_t* remove0(hash_set_t* this,void* element)
{
	remove1(this, element);
	return this;
}
bool exists(hash_set_t* self,void* element)
{
	bool success = remove1(self, element);
	if(success)
	{
		insert(self, element);
		return true;
	}
	return false;
}
void init0(hash_set_t* this,long capacity)
{
	bzero(this,sizeof(*this));
	this->members_size = capacity;
	this->members = calloc(capacity,sizeof(void*));
	for(long i=0;i<capacity;i++)
	{
		this->members[i] = calloc(1,sizeof(struct hash_set_node));
	}
};
static
void init(hash_set_t* this)
{
	init0(this,DEFAULT_CAPACITY);
}
static
void destory(hash_set_t* this)
{
	for(long i=0;i<this->members_size;i++)
	{
		free_node(this->members[i]);
	}
	free(this->members);
	bzero(this,sizeof(*this));
}

hash_set_t* hash_set_new()
{
	hash_set_t* self = malloc(sizeof(*self));
	init(self);
	self->insert = insert;
	self->remove = remove0;
	self->all_elements = all_elements;
	self->exists = exists;
	return self;
}
void hash_set_delete(hash_set_t* self)
{
	destory(self);
	free(self);
}
