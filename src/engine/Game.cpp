#include "Game.h"

Game::Game() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Blaster Master Remake");

  _world = World();

  _texHealthBar = TextureKeeper("Sprites/HealthBar.png");
  _texMessage = TextureKeeper("Sprites/Message.png");
  _texVictory = TextureKeeper("Sprites/Victory.png");

  _world.loadTextures({
    .player = "Sprites/SOPHIA.png",
    .playerMissle = "Sprites/Shot.png",
    .enemy = "Sprites/Enemy.png",
    .area = "maps/Area3.tmx",
  });
  _world.init();
}

Game::~Game() {
  CloseWindow();
}

void Game::update(float timePassed) {
  bool isLeftPressed = IsKeyDown(_keyLeft);
  bool isRightPressed = IsKeyDown(_keyRight);

  int dir = 0;
  if (isRightPressed) dir = 1;
  else if (isLeftPressed) dir = -1;

  if (isRightPressed || isLeftPressed) {
    _world.player->move(dir);
  }
  if (IsKeyDown(_keyJump)) _world.player->jump();
  if (IsKeyDown(_keyShoot)) _world.player->shoot();

  _world.update(timePassed);
}

void Game::draw() {
  _world.draw();

  // drawing healthBar
  const float HBx = 0;
  const float HBy = 5;
  const float HBx1 = HBx + 2;
  const float HBy1 = HBy + 4;
  const float bl = 25 / 64;
  float rate = (1 - bl) * ceil(_world.player->getHealth() + 8) * 0.0625 - 0.125;  //

  drawSprite(_texHealthBar, HBx, HBy, HBx1, HBy1, 0, 0, 0.5, 1);
  drawSprite(_texHealthBar, HBx, HBy, HBx1, HBy + 4 * rate, 0.5, 1 - rate, 1, 1);
  if (_world.player->isDead()) {
    drawSprite(_texMessage, 15, 15, 23, 23, 0, 0, 1, 1);
    if (IsKeyDown(_keyRestart)) {
      _world.init();
    }
  }

  _world.enemies.remove_if([](Enemy &enemy){
    return enemy.isDead();
  });
}

void Game::mainLoop() {
  this->update(GetFrameTime());
  BeginDrawing();
    ClearBackground(BLACK);
    draw();
  EndDrawing();
}