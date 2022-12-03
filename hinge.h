#ifndef HINGE_H
#define HINGE_H
#include "std.h"
typedef struct hinge hinge_t;
typedef struct passage passage_t;
void hinge_add_passage(passage_t*passage);
void hinge_remove_passage(passage_t* passage);
#endif
