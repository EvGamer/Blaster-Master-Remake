#include "Enemy.h"
#include <list>
#include <vector>

#pragma once
class EnemyFactory
{
  public:
  EnemyFactory(TextureKeeper texture, IWorld* world);
  Enemy create(float x, float y, char dirrection);

  private:
    TextureKeeper _texture;
    IWorld* _world;
};