#ifndef __SPRITE_PROVIDER
#define __SPRITE_PROVIDER

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

ALLEGRO_BITMAP *spriteProvider_get(spriteProvider *sp, char *path);

#endif