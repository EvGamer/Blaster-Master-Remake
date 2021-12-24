#include "Enemy.h"
#include <list>
#include <vector>

#pragma once
class EnemyFactory
{
  public:
  EnemyFactory(Texture2D texture, IWorld* world);
  Enemy create(float x, float y, char dirrection);

  private:
    Texture2D _texture;
    IWorld* _world;
};