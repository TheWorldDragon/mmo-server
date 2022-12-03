/*
 * author:dragon
 * time:Oct 29 2022
 *
 * realize a hash set of collection,
 * to complete dragon's developing game server!
 */
#ifndef HASH_SET_H
#define HASH_SET_H
#include <stdbool.h>
typedef void any_t;
typedef struct hash_set hash_set_t;
struct hash_set_node
{
	void* element;
	struct hash_set_node* next;
};
struct hash_set
{
	long number_of_member;
	struct hash_set_node** members;
	long members_size;
	hash_set_t*(*insert)(hash_set_t* self,any_t* element);
	hash_set_t*(*remove)(hash_set_t* self,any_t* element);
//elements is an array whose capacity should be members_size
	hash_set_t*(*all_elements)(hash_set_t* self,any_t*element);
	bool(*exists)(hash_set_t* self,any_t* element);
};
//init with capacity 10
hash_set_t* hash_set_new();
void hash_set_delete(hash_set_t* self);
#endif

