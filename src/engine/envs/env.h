#ifndef __ENV
#define __ENV

/*
 * Um ambiente(env) é uma maneira genérica de agrupar dados que pode ser implementadade maneiras diferentes comforme a necessidade,
 * E executar funcões baseado nisso - lista encadeada, fila de prioridades, arvore kd, etc
 */

typedef struct entity entity;
typedef struct game game;

typedef struct env env;
struct env {
    void *entities;
    int id;

    struct env *(*destroy) (struct env *e);

    int (*subscribe) (entity *e, struct env *target, int (*processFunc) (entity *e, game *g));
    int (*unsubscribe) (entity *e, struct env *target);
    entity *(*get) (int id, struct env *target);
    int (*compute) (struct env *e, game *g);
};

env *processEnv_create(int id);

#endif