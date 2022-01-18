#include "Game.h"
#include "../utils/Point.h"

namespace {
  const Rectangle MESSAGE_SOURCE = { 0, 0, 256, 256 };
  const ScreenVector MESSAGE_POSITION = { 200, 180 };
}

Game::Game():
  _window(MainWindow::init(WINDOW_WIDTH, WINDOW_HEIGHT, "Blaster Master Remake")),
  _healthBar("Sprites/HealthBar.png"),
  _endGameMessage("Sprites/Message.png"),
  _victoryMessage("Sprites/Victory.png")
{

  _world.loadTextures({
    .player = "Sprites/SOPHIA.png",
    .playerMissle = "Sprites/Shot.png",
    .enemy = "Sprites/Enemy.png",
    .area = "maps/Area3.tmx",
  });
  _world.init();
}

void Game::update(float timePassed) {
  bool isRightPressed = IsKeyDown(_keyRight);
  bool isLeftPressed = IsKeyDown(_keyLeft);
  uint8_t dirrection = 
    isRightPressed && isLeftPressed ? 0 :
    isLeftPressed ? FACING_LEFT :
    isRightPressed ? FACING_RIGHT :
    0
  ;

  auto& player = _world.player;

  if (dirrection) 
    player->move(dirrection);

  if (IsKeyDown(_keyJump))
    player->jump();
  
  if (IsKeyDown(_keyShoot))
    player->shoot();

  _world.update(timePassed);
  _healthBar.update(player->health() / PlayerConstants::MAX_HEALTH);

  if (!(player->isDead() && IsKeyDown(_keyRestart))) return;
  _world.init();
}

void Game::draw() {
  _world.draw();
  _healthBar.draw({ 20, 200 });

  if (_world.player->isDead()) {
    _endGameMessage.draw(MESSAGE_POSITION);
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