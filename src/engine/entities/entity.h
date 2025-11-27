#ifndef __ENTITY
#define __ENTITY

typedef struct hitbox hitbox;
typedef struct game game;
typedef struct env env;
typedef struct sprite sprite;

#include "vec.h"
#include <stdlib.h>

typedef struct entity entity;
struct entity {
    int id;
    vec pos;
    hitbox *hitbox;
    void *data;
    sprite *sprite;
    env **envs;
    size_t n_envs;

    int (*destroy) (entity *e, game *g);
};

entity *entity_create(game *G);
entity *entity_destroy(entity *e, game *g);

entity *entity_destroy_level(entity *e, game *g);


// Insere entidade em um ambiente qualquer
int subscribe(struct env *environment, entity *e, int (*process)(entity *, game *));

#endif