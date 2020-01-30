#pragma once
#include "../engine/IWorld.h"
#include "../utils/Rectangle.h"

namespace PlayerConstants {
  const float MAX_HEALTH = 8;
  const float MAX_JUMP_HEIGHT = 4;
  const float JUMP_SPEED = 0.08;
  const float JUMP_HEIGHT = 3;
  const float JUMP_SPEED_COEFFICIENT = 0.5; //ToDo replace with something more clear
  const float MAX_SPEED_X = 1;
  const float WALK_ACCELERATION = 0.1;
  const float DRAG_DECELLERATION_X = 0.05;
  const float SPEED_X_SCALE = 0.11; // ToDo replace with normal speed
  const int REQUIRED_FULL_JUMP_PRESSING_DURATION = 20;
  const int LIFE_WITHOUT_HEALTH_DURATION = 45;
  const int LIFE_WITHOUT_HEALTH_AFTER_LANDING_DURATION = 2;
  const float HIT_SPEED_COEFFICIENT = 0.5;
  const float HIT_ACCELERATION_Y = 1;
  const float COLLISION_MARGIN = 0.05;
  const float WALKING_ANIMATION_SPEED_THRESHOLD = 0.3;
  const float ANIMATION_FRAME_SIZE = 0.125; // of the texture
  const float DEATH_ANIMATION_FRAME_SIZE = 0.25;

  // this solves
  const float COLLISION_FORWARD_MARGIN = 0.1f; // ToDo find the solution to remove this hack

  //missle
  const float MISSLE_SPEED = 0.3;
  const float MISSLE_RELATIVE_Y = 0.6;
  const float MISSLE_RELATIVE_Y_FALLING = 0.4;
  const float MISSLE_RELATIVE_Y_JUMPING = 0.6;
}

using namespace PlayerConstants;

class Player {
 protected:
  GLuint _missleTextureId;
  GLuint _textureId;
  float _health = MAX_HEALTH;
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
  IWorld *_world;

  // ToDo create weapon class
  int _weaponCooldown = 0;
  float _missleRelativeInitialY;
  float _missleInitialX;
  MissleTraits _missleType;

 public:
  Player(float x, float y, GLuint textureId, GLuint missleTextureId,
         IWorld &world);
  
  inline Rect getRect() {
    return Rect(_x, _y, _width, _height);
  };
  void move(int dirrection);
  void jump();
  void hurt(float damage);
  inline float getFrontX() { return _missleInitialX; }
  inline float getX() { return _x; }
  inline float getY() { return _y; }
  void shoot();
  void tryHittingGround();
  void update();
  inline bool isDead() { return _deathAnimation.isEnded(); }
  void revive();
  void drawGizmo();
  inline bool collide(float ox, float oy) {
    return (ox > _x) && (ox < _x + _width) && (oy > _y) &&
           (oy < _y + _width);
  }
  void draw();
  inline float getHealth() {
    if (_health > 0) return _health;
    return 0;
  }
};
