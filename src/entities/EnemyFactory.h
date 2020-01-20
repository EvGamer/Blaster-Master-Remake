#include "Enemy.h"
#include <list>
#include <vector>

#pragma once
class EnemyFactory
{
  public:
  EnemyFactory(GLuint textureId, World* world);
  Enemy create(float x, float y, char dirrection);

  private:
    GLuint _textureId;
    World* _world;
};