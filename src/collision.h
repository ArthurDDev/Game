#ifndef __COLLISION
#define __COLLISION

#define HB_LEFT 1
#define HB_RIGHT 2
#define HB_TOP 4
#define HB_BOTTOM 8

typedef struct hitbox {
    vec offset, size;
} hitbox;

// Cria uma hitbox para uma entidade.
// Por padrão cria centralizada, mas pode ser especificada com flags
void hitbox_attatch(bool w, bool h, char flags);

// Retorna uma flag se ocorre colisão ou não
char collides(entity *a, entity *b);

// Retorna uma flag se colide com qualquer objeto ou não
char collides_any(entity *e, env *env);

#endif