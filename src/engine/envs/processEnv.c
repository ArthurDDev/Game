#include "env.h"

#include <stdlib.h>
#include <stdio.h>

#include "entity.h"
#include "game.h"

struct nodo_t {
    struct nodo_t *prox;
    int (*processFunc) (entity *e, game *g);
    entity *entity;
};

struct list_t {
    struct nodo_t *head;
    int tam;
};

int env_compute (struct env *e, game *g)
{
    if (!e || !e->entities || !g)
        return 1;

    if (!g->can_process)
        return 2;

    struct nodo_t *n = ((struct list_t *)e->entities)->head;
    struct nodo_t *n_prox = NULL;

    while (n && n->processFunc) {
        n_prox = n->prox;
        
        if (!g->can_process)
            return 2;

        if (n->processFunc(n->entity, g) != 0)
            printf("Erro ao processar entidade %d no ambiente %d\n", n->entity->id, e->id);

        n = n_prox;
    }

    return 0;
}

int env_subscribe (entity *e, struct env *target, int (*processFunc) (entity *e, game *g))
{
    if (!target || !target->entities || !e)
        return 1;

    struct nodo_t *n = ((struct list_t *)target->entities)->head;
    
    struct nodo_t *n_new = malloc(sizeof(struct nodo_t));

    n_new->entity = e;
    n_new->processFunc = processFunc;
    n_new->prox = NULL;

    while (n && n->prox)
        n = n->prox;

    if (n)
        n->prox = n_new;
    else
        ((struct list_t *)target->entities)->head = n_new;

    return ((struct list_t *)target->entities)->tam ++;
}

int env_unsubscribe (entity *e, struct env *target)
{
    if (!target || !target->entities || !e)
        return 1;

    struct nodo_t *n = ((struct list_t *)target->entities)->head;
    struct nodo_t *n_ant = NULL;

    while (n && n->prox && n->entity != e) {
        n_ant = n;
        n = n->prox;
    }

    if (!n || n->entity != e)
        return -1;

    if (n_ant)
        n_ant->prox = n->prox;
    else
        ((struct list_t *)target->entities)->head = n->prox;

    free(n);

    ((struct list_t *)target->entities)->tam--;
    return 0;
}

env *env_destroy (struct env *e)
{
    if (!e || !e->entities)
        return NULL;

    struct nodo_t *n = ((struct list_t *)e->entities)->head;
    struct nodo_t *n_aux;

    while (n) {
        n_aux = n->prox;
        free(n);
        n = n_aux;
    }

    
    free(e->entities);
    free(e);
    
    return NULL;
}

entity *env_get (int id, struct env *target)
{
    struct nodo_t *n = ((struct list_t *)target->entities)->head;

    while (n) {
        if (n->entity->id == id)
            return n->entity;
        
        n = n->prox;
    }

    return NULL;
}

env *processEnv_create(int id)
{
    struct env *e = malloc(sizeof(env));

    struct list_t *l = malloc(sizeof(struct list_t));
    l->head = NULL;
    l->tam = 0;
    
    e->id = id;
    e->destroy = env_destroy;
    e->subscribe = env_subscribe;
    e->unsubscribe = env_unsubscribe;
    e->compute = env_compute;
    e->get = env_get;

    e->entities = l;
    
    return e;
}
