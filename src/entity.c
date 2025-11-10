#include "entity.h"

#include <stdlib.h>

#include "vec.h"

entity *entity_create(vec pos)
{
    entity *e = malloc(sizeof(entity));
    e->pos = pos;
    e->sprite = NULL;
    e->data = NULL;
    e->hitbox = NULL;

    return e;
}

entity *entity_destroy(entity *e)
{
    if (e->data)
        free(e->data);
    
    free(e);
}