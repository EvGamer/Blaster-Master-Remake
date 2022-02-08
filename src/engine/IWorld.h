#pragma once
#include "../graphics/Animation.h"

struct ProjectileFactory {
  Animation burstAnim;
  Animation flyAnim;
  float spriteX;
  float spriteY;
  bool foe;
  float damage;
  TextureResource texture;
  bool canFall;
};

struct Projectile {
  Animation burstAnim;
  Animation flyAnim;
  float spriteX;
  float spriteY;
  float x;
  float y;
  float speedX;
  float speedY;
  bool hit;
  bool isFromEnemy;
  bool canFall;
  float damage;
};

class IWorld {
  public:
  void init();
  virtual void addProjectile(float x, float y, float speedX, float speedY, ProjectileFactory* projectileFactory) = 0;
  virtual float checkProjectileCollision(float x1, float y1, float x2, float y2, bool foe) = 0;
  virtual bool isSolidTileAtCoord(float x, float y) = 0;
  virtual float getGravity() = 0;
  virtual bool getTileFriction(unsigned char i, unsigned char j) = 0;
};