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

// Cria uma entidade e insere no ambiente global e do nivel, retorna essa entidade ou NULL em caso de erro
entity *entity_create(game *G);

// Destroi uma entidade, tirando de todos os ambientes e liberando memória, retorna sempre NULL
entity *entity_destroy(entity *e, game *g);

// Destroi uma entidade quando passando de nivel.
entity *entity_destroy_level(entity *e, game *g);


// Insere entidade em um ambiente qualquer
int subscribe(struct env *environment, entity *e, int (*process)(entity *, game *));

#endif