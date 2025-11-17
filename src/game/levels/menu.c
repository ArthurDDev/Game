#include "engine.h"

#include <stdio.h>

// Backdrop

typedef struct bddata {
    int position;
} bdata;

int render_backdrop(entity *e, game *G)
{
    if (!e || !G)
        return 1;

    bdata *data = (bdata *)e->data;
    e->sprite->current_frame = 0;
    e->sprite->timer = 0;
    for (int i = 0; i < 4; i++) {
        render_sprite(e->sprite, (data->position * i) % WW, 0, 3.0, 3.0);
        render_sprite(e->sprite, (data->position * i) % WW - WW, 0, 3.0, 3.0);
        e->sprite->current_frame = e->sprite->current_frame + 1;
    }

    return 0;
}

int process_backdrop(entity *e, game *G)
{
    if (!e || !G)
        return 1;

    bdata *data = (bdata *)e->data;
    data->position = (data->position + 1) % WW;

    return 0;
}

int create_backdrop(game *G)
{
    entity *backdrop = entity_create(G);
    struct bddata *data = malloc(sizeof(struct bddata));
    backdrop->data = data;
    data->position = 0;
    subscribe(G->envs[RENDER_ENV], backdrop, render_backdrop);
    subscribe(G->envs[PROCESS_ENV], backdrop, process_backdrop);

    backdrop->sprite = sprite_create(G, 4, vec_create(WW, HH), (const char *[]){
        "assets/background/back_0.png",
        "assets/background/back_1.png",
        "assets/background/back_2.png",
        "assets/background/back_3.png",
    }, SPR_TOP | SPR_LEFT);
    backdrop->sprite->delay = 10;

    return 0;
}

// Menu

typedef struct mdata {
    int selected;
    char **options;
} mdata;

int render_menu(entity *e, game *G)
{
    if (!e || !G)
        return 1;

    //al_draw_filled_rectangle(0, 0, WW, HH, C_BLUE);
    al_draw_text(G->font, C_WHITE, WW / 2, HH / 2 - 20, ALLEGRO_ALIGN_CENTRE, "Menu Principal");
    
    mdata *data = (mdata *)e->data;
    for (int i = 0; i < 3; i++) {
        ALLEGRO_COLOR color = (i == data->selected) ? C_RED : C_WHITE;
        al_draw_text(G->font, color, WW / 2, HH / 2 + i * 30, ALLEGRO_ALIGN_CENTRE, data->options[i]);
    }

    return 0;
}

int process_menu(entity *e, game *G)
{
    if (!e || !G)
        return 1;
    
    mdata *data = (mdata *)e->data;
    if (G->keys[ALLEGRO_KEY_UP]) {
        data->selected = (data->selected - 1 + 3) % 3;
        G->keys[ALLEGRO_KEY_UP] = 0;
    }
    if (G->keys[ALLEGRO_KEY_DOWN]) {
        data->selected = (data->selected + 1) % 3;
        G->keys[ALLEGRO_KEY_DOWN] = 0;
    }

    if (G->keys[ALLEGRO_KEY_ENTER]) {
        G->keys[ALLEGRO_KEY_ENTER] = 0;
        switch (data->selected) {
            case 0:
                level_change(G, 1);
                return 0;

            case 1:
                printf("Opcoes selecionado\n");
                // Lógica para abrir opções
                break;
            case 2:
                game_destroy(G);
                // Lógica para sair do jogo
                break;
        }
    }

    return 0;
}

int load_menu(game *G)
{
    create_backdrop(G);

    entity *menu = entity_create(G);
    struct mdata *data = malloc(sizeof(struct mdata));
    menu->data = data;
    data->selected = 0;
    data->options = malloc(3 * sizeof(char *));
    data->options[0] = strdup("Iniciar");
    data->options[1] = strdup("Opcoes");
    data->options[2] = strdup("Sair");
    subscribe(G->envs[RENDER_ENV], menu, render_menu);
    subscribe(G->envs[PROCESS_ENV], menu, process_menu);

    return 0;
}

int unload_menu(game *G)
{
    if (!G)
        return 1;

    return 0;
}