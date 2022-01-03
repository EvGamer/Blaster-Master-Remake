#pragma once
#include "../entities/Enemy.h"
#include "../entities/EnemyFactory.h"
#include "../graphics/TextureKeeper.h"
#include "World.h"
#include <list>

const unsigned COORD_UNIT = 32;
const unsigned WINDOW_WIDTH = 1024;
const unsigned WINDOW_HEIGHT = 640;
constexpr unsigned TILE_COLUMNS = WINDOW_WIDTH / COORD_UNIT;
constexpr unsigned TILE_ROWS = WINDOW_HEIGHT / COORD_UNIT;

class Game {
  public:
    Game();
    ~Game();

    void draw();
    void update(float timePassed);
    virtual void mainLoop();

  private:
    TextureKeeper _texHealthBar;
    TextureKeeper _texMessage;
    TextureKeeper _texVictory;
    World _world = World();

    int _keyLeft = KEY_A;
    int _keyRight = KEY_D;
    int _keyJump = KEY_W;
    int _keyShoot = KEY_SPACE;
    int _keyRestart = KEY_ENTER;
    int _camX = 8;
    int _camY = 7;
};
