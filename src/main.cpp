/**************************
 * Includes
 *
 **************************/
//

#include "engine/Game.h"

#if defined(PLATFORM_WEB)
  #include <emscripten/emscripten.h>

  void mainLoop(void* game) {
    static_cast<Game*>(game)->mainLoop();
  }
#endif

int main() {
  Game game;
  TraceLog(LOG_INFO, "INIT !!!!!!!!!!!!! HELLO WORLD!!!!!!!!!!!!!");

  #if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(mainLoop,  &game,  0, 1);
  #else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
      game.mainLoop();
    }
  #endif
}