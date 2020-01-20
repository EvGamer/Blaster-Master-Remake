#include "EnemyFactory.h"

EnemyFactory::EnemyFactory(GLuint textureId, World* world) {
  _textureId = textureId;
  _world = world;
};

Enemy EnemyFactory::create(float x, float y, char dirrection) {
  return Enemy(x, y, dirrection, _textureId, _world);
};