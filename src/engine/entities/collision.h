#ifndef __COLLISION
#define __COLLISION

typedef struct entity entity;
typedef struct env env;

#include "vec.h"

// Alinhamento da hitbox com relação a origem, exemplo:
// HB_LEFT | HB_LEFT Para no canto superior esquerdo
#define HB_LEFT 1
#define HB_RIGHT 2
#define HB_TOP 4
#define HB_BOTTOM 8

typedef struct hitbox hitbox;
struct hitbox {
    vec offset, size;
};

// Cria uma hitbox para uma entidade.
// Por padrão cria centralizada, mas pode ser especificada com flags
void hitbox_attatch(entity *e, double w, double h, char flags);

// Retorna uma flag se ocorre colisão ou não
char collides(entity *a, entity *b);

// Retorna a entidade que colidiu ou NULL
entity *collides_env(entity *e, env *env);

#endif