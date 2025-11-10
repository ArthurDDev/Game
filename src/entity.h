#ifndef __ENTITY
#define __ENTITY

#include "vec.h"
#include <allegro5/allegro5.h>

typedef struct entity {
    vec pos;
    ALLEGRO_BITMAP *sprite;
} entity;

entity *entity_create(vec pos);
entity *entity_destroy(entity *e);

#endif