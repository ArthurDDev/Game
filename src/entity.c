#include "entity.h"
#include "vec.h"
#include "stdlib.h"

entity *entity_create(vec pos, int (*process) (entity *e), int (*render) (entity *e))
{
    entity *e = malloc(sizeof(entity));
    e->pos = pos;
    e->process = process;
    e->render = render;

    return e;
}

entity *entity_destroy(entity *e)
{
    free(e);
}