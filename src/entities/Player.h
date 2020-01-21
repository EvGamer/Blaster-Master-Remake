#include "../engine/World.h"
#pragma once

namespace PlayerConstants {
  const float MAX_JUMP_HEIGHT = 4;
  const float JUMP_SPEED = 0.08;
  const float CANNON_Y = 0.6;
  const float CANNON_Y_FALLING = 0.4;
  const float CANNON_Y_JUMPING = 0.6;
  const float JUMP_HEIGHT = 3;
}

using namespace PlayerConstants;

class Player {
 protected:
  GLuint _missleTextureId;
  GLuint _textureId;
  float _health = 8;
  bool _isControlable;
  bool _isOnGround = true;
  float _initialX;
  float _initialY;
  float _x;
  float _y;
  float _width;
  float _height;
  char _dirrection;
  float _maxJumpHeight = MAX_JUMP_HEIGHT;
  float _jumpSpeed = JUMP_SPEED;
  float _speedX;
  float _speedY;

  // ToDo move that to what is inflicting damage
  float _hitDamage;
  int _jumpBeingPressedDuration;
  int _timeToLiveWithoutHealth = 0;
  float _halfJumpMaxY;
  unsigned _currentAnimationFrameIndex;
  Animation _standAnimation;
  Animation _walkAnimation;
  Animation _jumpAnimation;
  Animation _fallAnimation;
  Animation *_currentAnimation;
  Animation _deathAnimation;
  // unsigned char ca;
  // shotType _missleType;
  World *_world;

  // ToDo create weapon class
  int _weaponCooldown = 0;
  float _missleInitialY;
  float _missleInitialX;
  MissleType _missleType;

 public:
  Player(float x_in, float y_in, GLuint a_textureId, GLuint a_missleTextureId,
         World *world_link);

  void move(int dirrection);
  void jump();
  void hurt(float damage);
  inline float getFrontX() { return _missleInitialX; }
  inline float getX() { return _x; }
  inline float getY() { return _y; }
  void shoot();
  void continueFalling();
  void update();
  inline bool isDead() { return _deathAnimation.isEnded(); }
  void revive();
  void drawGizmo();
  inline bool collide(float ox, float oy) {
    return (ox > _x) && (ox < _x + _width) && (oy > _y) &&
           (oy < _y + _width);
  }
  void draw();
  inline float hull() {
    if (_health > 0) return _health;
    return 0;
  }
};
