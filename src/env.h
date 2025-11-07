#ifndef __ENV
#define __ENV

#include "entity.h"

typedef struct env {
    void *entities;

    struct env *(*root_destroy) (struct env *e);

    int (*env_insert) (entity *e, struct env *target);
    int (*env_remove) (entity *e, struct env *target);

    int (*env_compute) (struct env *e);
} env;

env *processEnv_create();

#endif