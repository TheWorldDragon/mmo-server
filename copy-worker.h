#ifndef COPY_WORKER_H
#define COPY_WORKER_H
#include "std.h"
typedef struct copy_worker copy_worker_t;
typedef struct passage passage_t;
struct copy_worker
{
	passage_t* passage;
	int src_fd,des_fd;
	copy_worker_t* pair;

};
copy_worker_t* copy_worker_new(int src_fd,int des_fd);
void copy_worker_delete(copy_worker_t* self);
void copy_worker_pair(copy_worker_t* copy_worker1,copy_worker_t* copy_worker2);
#endif
