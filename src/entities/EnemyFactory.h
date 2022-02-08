#include "Enemy.h"
#include <list>
#include <vector>

#pragma once
class EnemyFactory
{
  public:
  EnemyFactory(TextureResource texture, IWorld* world);
  Enemy create(float x, float y, char dirrection);

  private:
    TextureResource _texture;
    IWorld* _world;
};