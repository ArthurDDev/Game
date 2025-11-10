#ifndef __SPRITE
#define __SPRITE

#include <allegro5/allegro.h>

struct spriteNode {
    char *path;
    int count;
    struct spriteNode *prox;
    ALLEGRO_BITMAP *bitmap;
};

typedef struct spriteProvider {
    struct spriteNode *head;
} spriteProvider;

spriteProvider *spriteProvider_create();
spriteProvider *spriteProvider_destroy(spriteProvider *sp);

ALLEGRO_BITMAP *spriteProvider_get(spriteProvider *sp, const char *path);

#endif