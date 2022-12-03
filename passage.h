#ifndef PASSAGE_H
#define PASSAGE_H
#include "std.h"
typedef struct hinge hinge_t;
typedef void passage_child_t;
typedef struct passage passage_t;
struct passage
{
	passage_child_t *child;
	void (*pass)(passage_t* self);
	int fd;
};
#endif
