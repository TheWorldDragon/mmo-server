#include "all.h"
static void delete(copy_worker_t* self)
{
	free(self->passage);
	free(self);
}
static void disconnect_user(copy_worker_t* self)
{
	hinge_remove_passage(self->passage);
	close(self->src_fd);
	close(self->des_fd);
	delete(self);
	copy_worker_t* pair = self->pair;
	if(pair!=NULL)
	{
		hinge_remove_passage(pair->passage);
		close(pair->src_fd);
		close(pair->des_fd);
		delete(pair);
	}
}
static void pass0(copy_worker_t* self)
{
	char buf[1024];
	int len = read(self->src_fd,buf,1024);
	if(len<0)
	{
		fprintf(stderr,"copy worker read error\n");
	}
	else if(len==0)
	{

		disconnect_user(self);
		syslog(LOG_INFO,"A user disconnected.");
	}else
	{
		if(write(self->des_fd,buf,len)<=0)
		{
			disconnect_user(self);
			syslog(LOG_INFO,"A user disconnected.");
		}
	}
}
static void pass(passage_t* self)
{
	pass0(self->child);
}
copy_worker_t* copy_worker_new(int src_fd,int des_fd)
{
	copy_worker_t* self = calloc(1,sizeof(*self));
	self->passage = calloc(1,sizeof(*self->passage));
	self->src_fd = src_fd;self->des_fd = des_fd;
	self->passage->child = self;
	self->passage->fd = src_fd;
	self->passage->pass = pass;
	return self;
}
void copy_worker_delete(copy_worker_t* self)
{
	delete(self);
}

void copy_worker_pair(copy_worker_t* copy_worker1,copy_worker_t* copy_worker2)
{
	copy_worker1->pair = copy_worker2;
	copy_worker2->pair=copy_worker1;
}
