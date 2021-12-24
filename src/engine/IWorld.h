#pragma once
#include "../graphics/Animation.h"

struct MissleTraits {
  Animation burstAnim;
  Animation flyAnim;
  float spriteX;
  float spriteY;
  bool foe;
  float damage;
  Texture2D texture;
  bool falling;
};

struct Missle {
  Animation burstAnim;
  Animation flyAnim;
  float spriteX;
  float spriteY;
  float x;
  float y;
  float speedX;
  float speedY;
  bool hit;
  bool foe;
  bool falling;
  float damage;
};

class IWorld {
  public:
  void init();
  virtual void addMissle(float x, float y, float speedX, float speedY, MissleTraits* wpn) = 0;
  virtual float hit(float x1, float y1, float x2, float y2, bool foe) = 0;
  virtual bool isSolidTileAtCoord(float x, float y) = 0;
  virtual float getGravity() = 0;
  virtual bool getFrict(unsigned char i, unsigned char j) = 0;
};