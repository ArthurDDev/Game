#include <stdlib.h>

#include "env.h"
#include "entity.h"

/**
 * Implementação temporária como lista encadeada
 * 
 */

struct nodo_t {
    struct nodo_t *prox;
    entity *entity;
};

struct list_t {
    struct nodo_t *head;
    int tam;
};

int env_compute (struct env *e)
{
    if (!e || !e->entities)
        return 1;

    struct nodo_t *n = ((struct list_t *)e->entities)->head;

    while (n) {
        if (n->entity->process)
            n->entity->process(n->entity);
        n = n->prox;
    }

    return 0;
}

int env_insert (entity *e, struct env *target)
{
    if (!target || !target->entities || !e)
        return 1;

    struct nodo_t *n = ((struct list_t *)target->entities)->head;
    
    struct nodo_t *n_new = malloc(sizeof(struct nodo_t));

    n_new->entity = e;
    n_new->prox = NULL;

    while (n && n->prox)
        n = n->prox;

    if (n)
        n->prox = n_new;
    else
        ((struct list_t *)target->entities)->head = n_new;

    return ((struct list_t *)target->entities)->tam ++;
}

int env_remove (entity *e, struct env *target)
{
    if (!target || !target->entities || !e)
        return 1;

    struct nodo_t *n = ((struct list_t *)target->entities)->head;
    struct nodo_t *n_ant = NULL;

    while (n && n->prox && n->entity != e) {
        n_ant = n;
        n = n->prox;
    }

    if (!n || n->entity == e)
        return -1;

    if (n_ant)
        n_ant->prox = n->prox;
    else
        ((struct list_t *)target->entities)->head = n->prox;

    free(n);

    return ((struct list_t *)target->entities)->tam --;
}

env *root_destroy (struct env *e)
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

env *processEnv_create()
{
    struct env *e = malloc(sizeof(env));

    struct list_t *l = malloc(sizeof(struct list_t));
    l->head = NULL;
    l->tam = 0;
    
    e->root_destroy = root_destroy;
    e->env_insert = env_insert;
    e->env_remove = env_remove;
    e->env_compute = env_compute;

    e->entities = l;
    
    return e;
}
