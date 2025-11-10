#ifndef __ENTITY
#define __ENTITY

#include <allegro5/allegro5.h>

typedef struct hitbox hitbox;

#include "vec.h"

typedef struct entity entity;
struct entity {
    vec pos;
    hitbox *hitbox;
    void *data;
    ALLEGRO_BITMAP *sprite;
};

entity *entity_create(vec pos);
entity *entity_destroy(entity *e);

#endif