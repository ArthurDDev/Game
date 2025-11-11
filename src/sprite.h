#ifndef __SPRITE
#define __SPRITE

#define SPR_LEFT 1
#define SPR_RIGHT 2
#define SPR_TOP 4
#define SPR_BOTTOM 8

#include "vec.h"
#include <allegro5/allegro5.h>

typedef struct game game;

typedef struct sprite sprite;
struct sprite {
    vec origin;
    vec size;
    int delay;
    int current_frame;
    int timer;
    size_t n_images;
    ALLEGRO_BITMAP **images;
};

sprite *render_sprite(sprite *s, double x, double y, double scale_x, double scale_y);

sprite *sprite_create(game *g, size_t n_images, vec size, const char **image_paths, char flags);
sprite *sprite_destroy(sprite *s);

#endif