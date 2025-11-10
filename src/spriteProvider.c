#include "spriteProvider.h"
#include <allegro5/allegro.h>
#include <stdio.h>

spriteProvider *spriteProvider_create()
{
    spriteProvider *sp = malloc(sizeof(spriteProvider));
    sp->head = NULL;

    return sp;
}

spriteProvider *spriteProvider_destroy(spriteProvider *sp)
{
    struct spriteNode *n = sp->head;
    struct spriteNode *n_prox;

    while (n) {
        n_prox = n->prox;
        al_destroy_bitmap(n->bitmap);
        free(n->path);
        free(n);
        n = n_prox;
    }

    free(sp);

    return NULL;
}

ALLEGRO_BITMAP *spriteProvider_get(spriteProvider *sp, char *path)
{
    struct spriteNode *n = sp->head;

    while (n) {
        if (strcmp(n->path, path)) {
            n->count ++;
            return n->bitmap;
        }
        
        n = n->prox;
    }

    ALLEGRO_BITMAP *bitmap = al_load_bitmap(path);
    if (!bitmap) {
        fprintf(stderr, "Erro ao carregar sprite %s\n", path);
        return NULL;
    }

    n = malloc(sizeof(struct spriteNode));
    n->path = path;
    n->bitmap = bitmap;
    n->count = 1;
    n->prox = sp->head;
    sp->head = n;

    return bitmap;
}