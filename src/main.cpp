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

int keyLeft = KEY_LEFT;
int keyRight = KEY_RIGHT;
int keyJump = KEY_UP;
int keyShoot = KEY_KP_0;
int keyRestart = KEY_ENTER;
unsigned char n = 0;

/*********************
 * Function Decl
 *
 **************************/

void drawAndUpdate();

World world("maps/Area3.tmx");
world.setGlobalFriction(1);
world.init();
float camX = 8;
float camY = 7;

Texture2D texHealthBar = loadTexture("Sprites\\HealthBar.tga");
Texture2D texMessage = loadTexture("Sprites\\Message.tga");
Texture2D texVictory = loadTexture("Sprites\\Victory.tga");
Texture2D texBack = loadTextureL("Sprites\\Background.bmp");  //

void main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Blaster Master Remake");

  // testdraw();
  // DrawTile(0,1,0,0,world.get_texture());r//

  SetTargetFPS(60);

  #if defined(PLATFORM_WEB)
    emscripten_set_main_loop(updateDrawFrame, 0, 1);
  #else
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
      drawAndUpdate();
    }
  #endif
}

void drawAndUpdate() {
  static int dir = 0;
  /* OpenGL animation code goes here */
  static bool isLeftPressed = IsKeyPressed(keyLeft);
  static bool isRightPressed = IsKeyPressed(keyRight);
  int dir = 0;
  if (isRightPressed) dir = 1;
  else if (isLeftPressed) dir = -1;

  static int frameCounter = 0;
  if (frameCounter < 10) frameCounter++;
  else {
    if (isRightPressed || isLeftPressed) {
      world.player->move(dir);
    }
    if (IsKeyPressed(keyJump)) world.player->jump();
    if (IsKeyPressed(keyShoot)) world.player->shoot();

    world.update();  //
    for (auto& enemy : world.enemies) enemy.update(*world.player);
    frameCounter = 0;
  }


  // attempt to make camera move by whole pixels
  // world.player->drawGizmo();
  BeginDrawing();
  world.applyCamera();
    world.draw();
    for (auto& enemy : world.enemies) enemy.draw();
    world.enemies.remove_if([](Enemy &enemy){
      return enemy.isDead();
    });
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
      if (isKeyDown(keyRestart)) {
        world.init();
      }
    }
  EndMode2D();
  EndDrawing();
}
