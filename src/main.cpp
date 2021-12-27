/**************************
 * Includes
 *
 **************************/
//

#include "entities/Enemy.h"
#include "entities/EnemyFactory.h"
#include "engine/World.h"
#include <list>

#if defined(PLATFORM_WEB)
  #include <emscripten/emscripten.h>
#endif

const unsigned COORD_UNIT = 32;
const unsigned WINDOW_WIDTH = 1024;
const unsigned WINDOW_HEIGHT = 768;
constexpr unsigned TILE_COLUMNS = WINDOW_WIDTH / COORD_UNIT;
constexpr unsigned TILE_ROWS = WINDOW_HEIGHT / COORD_UNIT;


unsigned int key;

int keyLeft = KEY_A;
int keyRight = KEY_D;
int keyJump = KEY_W;
int keyShoot = KEY_SPACE;
int keyRestart = KEY_ENTER;
unsigned char n = 0;

Texture2D texHealthBar;
Texture2D texMessage;
Texture2D texVictory;
Texture2D texBack;

World world = World();
float camX = 8;
float camY = 7;

/*********************
 * Function Decl
 *
 **************************/
void drawAndUpdate(void);

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Blaster Master Remake");

  texHealthBar = loadTexture("Sprites/HealthBar.png");
  texMessage = loadTexture("Sprites/Message.png");
  texVictory = loadTexture("Sprites/Victory.png");
  texBack = loadTextureL("Sprites/Background.bmp");  //

  // world.setGlobalFriction(1);
  world.loadTextures({
    .player = "Sprites/SOPHIA.png",
    .playerMissle = "Sprites/Shot.png",
    .enemy = "Sprites/Enemy.png",
    .area = "maps/Area3.tmx",
  });
  world.init();

  // testdraw();
  // DrawTile(0,1,0,0,world.get_texture());r//

  #if defined(PLATFORM_WEB)
    emscripten_set_main_loop(drawAndUpdate, 60, 1);
  #else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
      drawAndUpdate();
    }
    CloseWindow();
  #endif
}

void drawAndUpdate(void) {
  static bool isLeftPressed = false;
  static bool isRightPressed = IsKeyDown(keyRight);
  if (IsKeyPressed(keyLeft)) isLeftPressed = true;
  else if (IsKeyReleased(keyLeft)) isLeftPressed = false;
  if (IsKeyPressed(keyRight)) isRightPressed = true;
  else if (IsKeyReleased(keyRight)) isRightPressed = false;

  int dir = 0;
  if (isRightPressed) dir = 1;
  else if (isLeftPressed) dir = -1;

  static int frameCounter = 0;

  world.update();  //
  if (isRightPressed || isLeftPressed) {
    world.player->move(dir);
  }
  if (IsKeyDown(keyJump)) world.player->jump();
  if (IsKeyDown(keyShoot)) world.player->shoot();

  for (auto& enemy : world.enemies) enemy.update(*world.player);


  // attempt to make camera move by whole pixels
  // world.player->drawGizmo();
  BeginDrawing();
    ClearBackground(BLACK);
    world.applyCamera();
      world.draw();
      for (auto& enemy : world.enemies) enemy.draw();
      world.enemies.remove_if([](Enemy &enemy){
        return enemy.isDead();
      });
    EndMode2D();

    // drawing healthBar
    const float HBx = 0;
    const float HBy = 5;
    const float HBx1 = HBx + 2;
    const float HBy1 = HBy + 4;
    const float bl = 25 / 64;
    float rate = (1 - bl) * ceil(world.player->getHealth() + 8) * 0.0625 - 0.125;  //

    drawSprite(texHealthBar, HBx, HBy, HBx1, HBy1, 0, 0, 0.5, 1);
    drawSprite(texHealthBar, HBx, HBy, HBx1, HBy + 4 * rate, 0.5, 1 - rate, 1, 1);
    if (world.player->isDead()) {
      drawSprite(texMessage, 15, 15, 23, 23, 0, 0, 1, 1);
      if (IsKeyDown(keyRestart)) {
        world.init();
      }
    }
  EndDrawing();
}