#include "level.h"
#include "stdio.h"

#include "game.h"
#include "env.h"

level *level_create(int (*load) (game *G), int (*unload) (game *G))
{
    level *l = malloc(sizeof(level));

    l->load = load;
    l->unload = unload;

    return l;
}

level *level_destroy(level *l)
{
    if (!l)
        return NULL;

    free(l);

    return NULL;
}

void level_change(game *g, int next_level)
{
    if (g->cur_level->unload)
        g->cur_level->unload(g);
    
    g->envs[LEVEL_ENV]->compute(g->envs[LEVEL_ENV], g);
    g->envs[LEVEL_ENV]->destroy(g->envs[LEVEL_ENV]);
    g->envs[LEVEL_ENV] = processEnv_create(LEVEL_ENV);


    for (int i = 6; i < g->n_envs; i++) {
        g->envs[i]->destroy(g->envs[i]);
        g->envs[i] = NULL;
    }

    g->n_envs = 6;

    g->cur_level = g->levels[next_level];
    g->cur_level_id = next_level;
    g->can_process = 0;
    g->cur_level->load(g);

}
