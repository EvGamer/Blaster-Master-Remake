#include "Game.h"
#include "../utils/Point.h"

const Rectangle MESSAGE_SOURCE = { 0, 0, 256, 256 };
const ScreenVector MESSAGE_POSITION = { 200, 180 };

Game::Game():
  _window(MainWindow::init(WINDOW_WIDTH, WINDOW_HEIGHT, "Blaster Master Remake")),
  _healthBar("Sprites/HealthBar.png"),
  _endGameMessage("Sprites/Message.png", MESSAGE_SOURCE),
  _victoryMessage("Sprites/Victory.png", MESSAGE_SOURCE)
{

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
  _healthBar.update(_world.player->getHealth() / PlayerConstants::MAX_HEALTH);
}

void Game::draw() {
  _world.draw();
  _healthBar.draw({ 20, 200 });

  if (_world.player->isDead()) {
    _endGameMessage.draw(MESSAGE_POSITION);
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