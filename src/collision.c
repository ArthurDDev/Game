#include "collision.h"

#include "entity.h"

void hitbox_attatch(entity *e, double w, double h, char flags)
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

    vec posA1 = vec_add(a->hitbox->offset, a->pos);
    vec posA2 = vec_add(vec_add(a->hitbox->offset, a->pos), a->hitbox->size);

    vec posB1 = vec_add(b->hitbox->offset, b->pos);
    vec posB2 = vec_add(vec_add(b->hitbox->offset, b->pos), b->hitbox->size);
    if ((posB1.x < posA2.x && posB2.x > posA1.x &&
        posB1.y < posA2.y && posB2.y > posA1.y))
        return 1;

    return 0;
}

char collides_any(entity *e, env *env)
{
    return 0;
}
