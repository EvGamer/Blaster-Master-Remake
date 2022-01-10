#include "Game.h"
#include "../graphics/TextureFragment.h"
#include "../utils/Point.h"

Game::Game() {
  _window = MainWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Blaster Master Remake");

  _healthBar = FillBar(
    "Sprites/HealthBar.png",
    { 12, 7, 9, 50 },
    { 45, 8, 6, 31 },
    { 1, 1 }
  );
  _endGameMessage = ScreenTextureFragment("Sprites/Message.png", { 0, 0, 256, 256 });
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
  TraceLog(LOG_INFO, "[GAME] Closed");
}

void Game::update(float timePassed) {
  bool isLeftPressed = IsKeyDown(_keyLeft);
  bool isRightPressed = IsKeyDown(_keyRight);

  int dir = 0;
  if (isRightPressed) dir = FACING_RIGHT;
  else if (isLeftPressed) dir = FACING_LEFT;

  if (isRightPressed || isLeftPressed) {
    _world.player->move(dir);
  }
  if (IsKeyDown(_keyJump)) _world.player->jump();
  if (IsKeyDown(_keyShoot)) _world.player->shoot();

  _world.update(timePassed);
  _healthBar.update(_world.player)
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
    _endGameMessage.draw({ 200, 180 });
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