/*
 * author:dragon
 * Oct. 30 19:47 2022
 * Linear algorithm of disjoint set.
 */
#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H
typedef struct 
{
	long* members;
	long members_size;
}disjoint_set_t;
void disjoint_set_init(disjoint_set_t* ,long number_of_member);
long disjoint_set_find(disjoint_set_t* , long id);
void disjoint_set_union(disjoint_set_t* ,long id1,long id2);
void disjoint_set_destory(disjoint_set_t* );
#endif
