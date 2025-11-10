#ifndef __ENTITY
#define __ENTITY

#include <allegro5/allegro5.h>

typedef struct hitbox hitbox;
typedef struct game game;
typedef struct env env;

#include "vec.h"

typedef struct entity entity;
struct entity {
    vec pos;
    hitbox *hitbox;
    void *data;
    ALLEGRO_BITMAP *sprite;
    env **envs;
    size_t n_envs;
};

entity *entity_create(game *G);
entity *entity_destroy(entity *e);

// Insere entidade em um ambiente qualquer
int subscribe(struct env *environment, entity *e, int (*process)(entity *));

#endif