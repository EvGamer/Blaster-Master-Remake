#include "EnemyFactory.h"

EnemyFactory::EnemyFactory(Texture texture, IWorld* world) {
  _texture = texture;
  _world = world;
};

Enemy EnemyFactory::create(float x, float y, char dirrection) {
  return Enemy(x, y, dirrection, _texture, _world);
};