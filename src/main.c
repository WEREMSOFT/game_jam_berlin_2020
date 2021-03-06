#include <stdlib.h>
#define DEBUG_MEMORY
#include "utils/debug.h"


#include "raylib.h"

#ifdef PI
#undef PI
#endif
#define SP_DRAW_DOUBLE_FACED

#define SP_LAYER_SPACING 1
#define SP_LAYER_SPACING_BASE 2
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS

#include <spine-raylib.h>
#include "raygui.h"

#include <spine/spine.h>
#include <flecs.h>
//#include <flecs_systems_admin/flecs_systems_admin.h>

#include "utils/structs.h"
#include "utils/common.h"
#include "utils/assets.h"

ecs_world_t *screens[SCREEN_COUNT] = {0};

// Screens
#include "screens/game.h"
#include "screens/main_menu.h"

game_context_t game_context = {0};

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
void game_update() {
    ecs_progress(game_context.world, 0);
}
#endif

int main(int argc, char *argv[]) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Jam 2020 Berlin - Rebuild");
    init_assets();

    for (int i = 0; i < SCREEN_COUNT; i++) {
        screens[i] = ecs_init_w_args(argc, argv);
    }

    SetTargetFPS(60);
    game_context = init_game_context();

    game_world_init(screens[SCREEN_GAME], &game_context);
    init_main_menu_world(screens[SCREEN_MAIN_MENU], &game_context);

    game_context.world = screens[SCREEN_GAME];

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(game_update, 0, 1);
#else
    while (ecs_progress(game_context.world, 0) && !WindowShouldClose());

    for (int i = 0; i < SCREEN_COUNT; i++) {
        ecs_fini(screens[i]);
    }

    destroy_assets();
    CloseWindow();        // Close window and OpenGL context
#endif
}