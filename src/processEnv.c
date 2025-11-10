#include <stdlib.h>
#include <stdio.h>

#include "env.h"
#include "entity.h"

/**
 * Implementação temporária como lista encadeada
 * 
 */

struct nodo_t {
    struct nodo_t *prox;
    int (*processFunc) (entity *e);
    entity *entity;
};

struct list_t {
    struct nodo_t *head;
    int tam;
};

int compute (struct env *e)
{
    if (!e || !e->entities)
        return 1;

    struct nodo_t *n = ((struct list_t *)e->entities)->head;

    while (n) {
        n->processFunc(n->entity);
        n = n->prox;
    }

    return 0;
}
int subscribe (entity *e, struct env *target, int (*processFunc) (entity *e))
{
    if (!target || !target->entities || !e || !processFunc)
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

int unsubscribe (entity *e, struct env *target)
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

env *destroy (struct env *e)
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
    
    e->destroy = destroy;
    e->subscribe = subscribe;
    e->unsubscribe = unsubscribe;
    e->compute = compute;

    e->entities = l;
    
    return e;
}
