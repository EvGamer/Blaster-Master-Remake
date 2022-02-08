#pragma once
#include "../entities/Enemy.h"
#include "../entities/EnemyFactory.h"
#include "../graphics/TextureResource.h"
#include "../graphics/Sprite.h"
#include "../ui/FillBar.h"
#include "World.h"
#include "MainWindow.h"
#include "constants.h"
#include <list>


class Game {
  public:
    Game();

    void draw();
    void update(float timePassed);
    virtual void mainLoop();

  private:
    MainWindowPtr _window;
    TextureResource _texVictory;
    ScreenSprite _endGameMessage;
    ScreenSprite _victoryMessage;
    BrickFillBar _healthBar;
    World _world = World();

    int _keyLeft = KEY_A;
    int _keyRight = KEY_D;
    int _keyJump = KEY_W;
    int _keyShoot = KEY_SPACE;
    int _keyRestart = KEY_ENTER;
    int _camX = 8;
    int _camY = 7;
};
