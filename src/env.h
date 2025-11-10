#ifndef __ENV
#define __ENV

typedef struct entity entity;

typedef struct env env;
struct env {
    void *entities;
    int id;

    struct env *(*destroy) (struct env *e);

    int (*subscribe) (entity *e, struct env *target, int (*processFunc) (entity *e));
    int (*unsubscribe) (entity *e, struct env *target);

    int (*compute) (struct env *e);
};

env *processEnv_create(int id);

#endif