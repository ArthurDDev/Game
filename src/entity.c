#include "entity.h"

#include <stdlib.h>

#include "vec.h"
#include "game.h"
#include "env.h"
#include "entity.h"

int destroy_entity(entity *e, game *g)
{
    entity_destroy(e);
    return 0;
}

int destroy_entity_level(entity *e, game *g)
{
    entity_destroy_level(e);
    return 0;
}

entity *entity_create(game *G)
{
    entity *e = malloc(sizeof(entity));
    e->pos = vec_create(0.0, 0.0);
    e->sprite = NULL;
    e->data = NULL;
    e->hitbox = NULL;
    e->envs = malloc(sizeof(env *) * 1);
    e->envs[0] = G->envs[MASTER_ENV];
    e->n_envs = 1;

    e->id = G->envs[MASTER_ENV]->subscribe(e, G->envs[MASTER_ENV], destroy_entity);
    subscribe(G->envs[LEVEL_ENV], e, destroy_entity_level);

    return e;
}

entity *entity_destroy(entity *e)
{
    if (e->data)
        free(e->data);
 
    if (e->hitbox)
        free(e->hitbox);

    for (size_t i = 0; i < e->n_envs; i++) {
        if (e->envs[i]->id != MASTER_ENV)
            e->envs[i]->unsubscribe(e, e->envs[i]);
    }
    free(e->envs);

    free(e);
}

entity *entity_destroy_level(entity *e)
{
    if (e->data)
        free(e->data);
 
    if (e->hitbox)
        free(e->hitbox);

    for (size_t i = 0; i < e->n_envs; i++) {
        if (e->envs[i]->id != LEVEL_ENV)
            e->envs[i]->unsubscribe(e, e->envs[i]);
    }
    free(e->envs);

    free(e);
}

int subscribe(struct env *environment, entity *e, int (*process)(entity *, game *))
{   
    for (int i = e->n_envs - 1; i >= 0; i--) {
        if (e->envs[i] == environment)
            return 0;
    }

    e->envs = realloc(e->envs, sizeof(env *) * (e->n_envs + 1));
    e->envs[e->n_envs] = environment;
    e->n_envs += 1;

    environment->subscribe(e, environment, process);

    return 0;
}