#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "disjoint-set.h"

static long find(long* members, long id)
{
	if(members[id]==-1)
	{
		return id;
	}
	return members[id] = find(members,members[id]);
};
void disjoint_set_init(disjoint_set_t* this,long number_of_member)
{
	bzero(this,sizeof(*this));
	this->members_size = number_of_member;
	this->members = malloc(sizeof(*this->members)*number_of_member);
	for(long i=0;i<number_of_member;i++)
	{
		this->members[i] = -1;
	}
}
long disjoint_set_find(disjoint_set_t* this, long id)
{
	long root = find(this->members,id);
	return root;
}
void disjoint_set_union(disjoint_set_t* this,long id1,long id2)
{
	if(id1==id2)
		return;
	long root1=find(this->members,id1);
	long root2=find(this->members,id2);
	if(root1==root2)
		return;
	this->members[root2] = root1;
}
void disjoint_set_destory(disjoint_set_t* this)
{
	free(this->members);
	bzero(this,sizeof(*this));
	return; 
}

int main()
{
	disjoint_set_t* d = malloc(sizeof(*d));
	disjoint_set_init(d,1000);
	for(int i=1;i<100;i++)
	{
		disjoint_set_union(d,i-1,i);
	}
	for(int i=100;i<200;i++)
	{
		disjoint_set_union(d,i-1,i);
	}
	for(int i=1;i<300;i++)
	{
		disjoint_set_union(d,i-1,i);
	}
	disjoint_set_union(d,2,11);
	disjoint_set_destory(d);
}
