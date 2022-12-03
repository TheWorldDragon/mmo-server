#ifndef USER_ACCEPTOR_H
#define USER_ACCEPTOR_H
#include "std.h"
typedef struct passage passage_t;
typedef struct user_acceptor user_acceptor_t;

struct user_acceptor
{
	passage_t* passage;
};
user_acceptor_t* user_acceptor_new(int server_fd);
void user_acceptor_delete(user_acceptor_t *self);
#endif
