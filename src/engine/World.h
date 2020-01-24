#pragma once
#include <list>
#include <array>

#include "../graphics/Animation.h"
#include "../typeAliases.h"
#include "mapArray.h"
#include "TileTraits.h"
#include "Map.h"


struct MissleTraits {
  Animation burstAnim;
  Animation flyAnim;
  float spriteX;
  float spriteY;
  bool foe;
  float damage;
  GLuint textureId;
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

class World {
 private:
  float _friction;
  float _gravity;

  Map _map;

  GLuint _texture;
  // Map, hardcoded in haste

  std::list<Missle> _missles;

 public:
  void addMissle(float x, float y, float speedX, float speedY, MissleTraits *wpn);
  float hit(float x1, float y1, float x2, float y2, bool foe);
  bool collide(float x, float y);
  inline float getGravity() {
    return _gravity;
  };
  inline void setGravity(float a_gravity) {
    _gravity = a_gravity;
  };
  bool getFrict(unsigned char i, unsigned char j);
  void update();
  inline void setGlobalFriction(float a_friction) {
    _friction = a_friction;
  };
  void setSolid(unsigned first, unsigned last);
  void drawLevel(float scrX, float scrY);
  World(String fileName);
  ~World();
};
