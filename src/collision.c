#ifndef __COLLISION
#define __COLLISION

#include "collision.h"

void hitbox_attatch(entity *e, bool w, bool h, char flags)
{
    hitbox *hb = malloc(sizeof(hitbox));

    hb->offset = vec_create(0.0, 0.0);    
    hb->size = vec_create(w, h);

    if (e->hitbox)
        free(e->hitbox);

    e->hitbox = hb;
}

char collides(entity *a, entity *b)
{
    if (!a || !b || !a->hitbox || !b->hitbox)
        return 0;

    hitbox hba = a->hitbox;
    hitbox hbb = b->hitbox;

    double ax1 = hba->offset->x1

    if ()
}

char collides_any(entity *e, env *env)
{
    return 0;
}

#endif