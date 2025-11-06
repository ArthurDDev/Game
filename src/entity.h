#ifndef __ENTITY
#define __ENTITY

#include "vec.h"

typedef struct entity {
    vec pos;
    int (*process) (struct entity *e);
    int (*render) (struct entity *e);
} entity;

entity *entity_create(vec pos, int (*process) (entity *e), int (*render) (entity *e));
entity *entity_destroy(entity *e);

#endif