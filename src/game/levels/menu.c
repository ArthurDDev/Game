#include "engine.h"

#include <stdio.h>

#include "controller.h"

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
        render_sprite(e->sprite, (data->position * i) % WW, 0, 6.0, 6.0);
        render_sprite(e->sprite, (data->position * i) % WW - WW, 0, 6.0, 6.0);
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

enum menuMode {
    MAIN_MENU,
    GAMEOVER_MENU,
    WIN_MENU,
};

typedef struct mdata {
    int selected;
    int mode;
    int n_options;
    char **options;
} mdata;

int render_menu(entity *e, game *G)
{
    if (!e || !G)
        return 1;
    
    mdata *data = (mdata *)e->data;
    
    switch (data->mode) {
        case MAIN_MENU:
            al_draw_text(G->font, C_WHITE, WW / 2, HH / 2 - 20, ALLEGRO_ALIGN_CENTRE, "Menu Principal");

            break;
        case GAMEOVER_MENU:
            al_draw_text(G->font, C_WHITE, WW / 2, HH / 2 - 20, ALLEGRO_ALIGN_CENTRE, "Skill issue?");

            break;
        
        case WIN_MENU:
            al_draw_text(G->font, C_WHITE, WW / 2, HH / 2 - 20, ALLEGRO_ALIGN_CENTRE, "Você venceu!");

            break;
    }
    
    for (int i = 0; i < data->n_options; i++) {
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
        data->selected = (data->selected - 1 + data->n_options) % data->n_options;
        G->keys[ALLEGRO_KEY_UP] = 0;
    }
    if (G->keys[ALLEGRO_KEY_DOWN]) {
        data->selected = (data->selected + 1) % data->n_options;
        G->keys[ALLEGRO_KEY_DOWN] = 0;
    }

    
    if (G->keys[ALLEGRO_KEY_ENTER]) {
        G->keys[ALLEGRO_KEY_ENTER] = 0;
        
        if (!strcmp(data->options[data->selected], "Iniciar"))
            level_change(G, 1);
        else if (!strcmp(data->options[data->selected], "Continuar")) {
            struct controllerData *cdata = (struct controllerData *)G->controller->data;
            cdata->life = cdata->maxLife;
            level_change(G, 1);
        }
        else if (!strcmp(data->options[data->selected], "Voltar ao menu")) {
            G->controller = entity_destroy(G->controller, G);
            level_change(G, 0);
        }
        else if (!strcmp(data->options[data->selected], "Opções"))
            printf("Menu de opções selecionado\n");
        else if (!strcmp(data->options[data->selected], "Sair"))
            game_destroy(G);

    }

    return 0;
}

int load_menu(game *G)
{
    create_backdrop(G);
    
    entity *menu = entity_create(G);
    struct mdata *data = malloc(sizeof(struct mdata));
    menu->data = data;
    subscribe(G->envs[RENDER_ENV], menu, render_menu);
    subscribe(G->envs[PROCESS_ENV], menu, process_menu);

    if (G->controller == NULL) {
        data->n_options = 3;
        data->selected = 0;
        data->mode = MAIN_MENU;
        data->options = malloc(3 * sizeof(char *));
        data->options[0] = strdup("Iniciar");
        data->options[1] = strdup("Opcoes");
        data->options[2] = strdup("Sair");
    }
    else{
        struct controllerData *cdata = (struct controllerData *)G->controller->data;
        
        if (cdata->life == 0) {
            data->mode = GAMEOVER_MENU;
            data->n_options = 2;
            data->selected = 0;
            data->options = malloc(2 * sizeof(char *));
            data->options[0] = strdup("Continuar");
            data->options[1] = strdup("Sair");
        }
        else {
            data->mode = WIN_MENU;
            data->n_options = 2;
            data->selected = 1;
            data->options = malloc(2 * sizeof(char *));
            data->options[0] = strdup("Voltar ao menu");
            data->options[1] = strdup("Sair");
        }

        
    }
    
    return 0;
}

int unload_menu(game *G)
{
    if (!G)
        return 1;

    return 0;
}