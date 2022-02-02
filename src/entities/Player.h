#pragma once
#include "../engine/IWorld.h"
#include "../engine/TileTraits.h"
#include "../engine/Timer.h"
#include "../utils/Rectangle.h"
#include "../utils/Point.h"
#include "Entity.h"

namespace PlayerConstants {
  constexpr float WIDTH = 26.0 / 16;
  //constexpr float HEIGHT = 18.0 / 16;
  constexpr float HEIGHT = 1;
  const float MAX_HEALTH = 8;

  const float MAX_JUMP_HEIGHT = 4;
  const float SHORT_JUMP_HEIGHT = 3;
  const float JUMP_SPEED_RELATIVE_TO_HEIGHT = 4.8;
  const float INITIAL_JUMP_SPEED_Y = JUMP_SPEED_RELATIVE_TO_HEIGHT * MAX_JUMP_HEIGHT;
  const float GRAVITY_SPEED_INCREASE_Y = (
    0.5
    * JUMP_SPEED_RELATIVE_TO_HEIGHT
    * INITIAL_JUMP_SPEED_Y
  );

  const float MAX_SPEED_X = 6.6;
  const float WALK_SPEED_INCREASE = 6 * MAX_SPEED_X;
  const float DRAG_SPEED_DECREASE_X = 0.5 * WALK_SPEED_INCREASE;
  const float REQUIRED_FULL_JUMP_PRESSING_DURATION = 0.4;
  const float LIFE_WITHOUT_HEALTH_DURATION = 45;
  const float LIFE_WITHOUT_HEALTH_AFTER_LANDING_DURATION = 2;
  const float HIT_SPEED_COEFFICIENT = 30;
  const float HIT_ACCELERATION_Y = 6.6;
  const float COLLISION_MARGIN = 0.05;
  const float WALKING_ANIMATION_SPEED_THRESHOLD = 2.0f;
  const float ANIMATION_FRAME_SIZE = 0.125; // of the texture
  const float DEATH_ANIMATION_FRAME_SIZE = 0.25;
  const float WEAPON_COOLDOWN = 15;

  // this solves
  const float COLLISION_FORWARD_MARGIN = 0.1f; // ToDo find the solution to remove this hack

  //missle
  const float MISSLE_SPEED = 0.3;
  const float MISSLE_RELATIVE_Y = 0.6;
  const float MISSLE_RELATIVE_Y_FALLING = 0.4;
  const float MISSLE_RELATIVE_Y_JUMPING = 0.6;
}


class Player : public Entity {
 protected:
  TextureKeeper _missleTexture;
  float _health = PlayerConstants::MAX_HEALTH;
  float _accelerationY = 0;
  bool _isControlable = true;
  bool _isOnGround = true;
  float _maxJumpHeight = PlayerConstants::MAX_JUMP_HEIGHT;

  // ToDo move that to what is inflicting damage
  float _hitDamage;
  float _jumpBeingPressedDuration;
  float _timeToLiveWithoutHealth = 0;
  float _halfJumpMaxY;
  unsigned _currentAnimationFrameIndex;
  Animation _standAnimation;
  Animation _walkAnimation;
  Animation _jumpAnimation; 
  Animation _fallAnimation;
  Animation _deathAnimation;
  // unsigned char ca;
  // shotType _missleType;
  IWorld *_world;

  // ToDo create weapon class
  float _weaponCooldown = 0;
  MissleTraits _missleType;

 public:
  Player(
    float x, float y,
    TextureKeeper texture, TextureKeeper missleTexture,
    IWorld &world
  );
  
  inline Rect rect() {
    return Rect(_x, _y, _width, _height);
  };

  float speedX();
  void move(float timePassed, int dirrection);
  void jump(float timePassed);
  void takeDamage(float damage);
  Animation& getCurrentAnimation();
  inline float missleInitialY() {
    using namespace PlayerConstants;
    if (_speedY == 0) return MISSLE_RELATIVE_Y;
    return _speedY > 0 ? MISSLE_RELATIVE_Y_JUMPING : MISSLE_RELATIVE_Y_FALLING;
  }
  inline float missleInititalX() {
    return _dirrection < 0 ? _x : _x + _width;
  }
  void shoot();
  void onTileCollision(WorldVector correction);
  void update(float timePassed);
  inline bool isDead() { return _deathAnimation.isStopped(); }
  void drawGizmo();
  inline bool isPointWithin(float ox, float oy) {
    return
      (ox > _x) && (ox < _x + _width)
      && (oy > _y) && (oy < _y + _width);
  }
  void draw();
  inline float health() {
    return _health > 0 ? _health : 0;
  }
};
