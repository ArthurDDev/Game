#ifndef __ENV
#define __ENV

#include "entity.h"

typedef struct env {
    void *entities;

    struct env *(*destroy) (struct env *e);

    int (*subscribe) (entity *e, struct env *target, int (*processFunc) (entity *e));
    int (*unsubscribe) (entity *e, struct env *target);

    int (*compute) (struct env *e);
} env;

env *processEnv_create();

#endif