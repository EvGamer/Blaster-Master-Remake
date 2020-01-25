#pragma once
#include "Player.h"

namespace EnemyConstants {
  const float SPEED_X = 1;
}

using namespace EnemyConstants;

class Enemy {
 private:
  float _health;
  bool _isHit;
  float _x;
  float _y;
  float _width;
  float _height;
  float _speedX = SPEED_X;
  float _speedY;
  char _dirrection;
  Animation _deathAnimation;
  Animation _walkAnimation;
  Animation _shootAnimation;
  Animation *_currentAnimation;
  GLuint _textureId;
  IWorld *_world;

  //ToDo replace by weapon class
  int _weaponCooldown;
  char _weaponMagazine = 0;
  int _weaponReloadCooldown;
  MissleTraits _missleType;

 public:
  Enemy(float a_x, float a_y, char a_dirrection, GLuint a_textureId, IWorld *a_world);
  inline bool isDead() { return _deathAnimation.isEnded(); }
  void kill();
  void update(Player &p1);
  void draw();
};