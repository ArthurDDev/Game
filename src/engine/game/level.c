#include "level.h"

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
/*
    for (int i = 1; i < g->n_envs; i++) {
        g->envs[i]->destroy(g->envs[i]);
    }

    g->envs[PROCESS_ENV] = processEnv_create(PROCESS_ENV);
    g->envs[ASYNC_PROCESS_ENV] = processEnv_create(ASYNC_PROCESS_ENV);
    g->envs[RENDER_ENV] = processEnv_create(RENDER_ENV);
*/
    
    g->cur_level = g->levels[next_level];
    g->cur_level->load(g);
    g->can_process = 0;

}
