#include "sprite.h"

#include <allegro5/allegro.h>
#include <stdio.h>

#include "vec.h"
#include "game.h"
#include "spriteProvider.h"

sprite *render_sprite(sprite *s, double x, double y, double scale_x, double scale_y)
{
    s->timer += 1;
    if (s->timer >= s->delay) {
        s->current_frame = (s->current_frame + 1) % s->n_images;
        s->timer = 0;
    }
   
    al_draw_scaled_bitmap(
        s->images[s->current_frame],
        0, 0,
        s->size.x, s->size.y,
        x - (s->origin.x * scale_x), y - (s->origin.y * scale_y),
        s->size.x * scale_x, s->size.y * scale_y,
        0
    );
}

sprite *sprite_create(game *g, size_t n_images, vec size, const char **image_paths, char flags)
{
    ALLEGRO_BITMAP **images = malloc(sizeof(ALLEGRO_BITMAP *) * n_images);
    for (size_t i = 0; i < n_images; i++) {
        images[i] = spriteProvider_get(g->sprites, image_paths[i]);
        
        if (!images[i]) {
            printf("Erro ao carregar imagem: %s\n", image_paths[i]);
            free(images);
            return NULL;
        }
    }

    sprite *s = malloc(sizeof(sprite));

    s->origin = vec_create(size.x / 2.0,size.y / 2.0);
    if (flags & SPR_LEFT)
        s->origin.x = 0;
    else if (flags & SPR_RIGHT)
        s->origin.x = size.x;

    if (flags & SPR_TOP)
        s->origin.y = 0;
    else if (flags & SPR_BOTTOM)
        s->origin.y = size.y;
    
    s->size = size;
    s->delay = 5;
    s->n_images = n_images;
    s->current_frame = 0;
    s->timer = 0;
    s->images = images;
    return s;
}

sprite *sprite_destroy(sprite *s)
{
    if (!s)
        return NULL;

    free(s->images);
    free(s);

    return NULL;
}
