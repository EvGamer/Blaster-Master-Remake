#include "Player.h"

using namespace PlayerConstants;

void Player::move(float timePassed, int dirrection) {
  if (!_isControlable) return;
  _dirrection = dirrection;
  _speedX += copysignf(WALK_ACCELERATION, _dirrection) * timePassed;
  if (abs(_speedX) > MAX_SPEED_X) _speedX = copysignf(MAX_SPEED_X, _speedX);
  // wheels should only turn then player presses the button
  getCurrentAnimation().resume();
}

void Player::jump(float timePassed) {
  if (!_isControlable) return;
  if (!_isOnGround) {
    _jumpBeingPressedDuration -= timePassed;
    return; 
  }
  _speedY = INITIAL_JUMP_SPEED_Y;
  _jumpBeingPressedDuration = REQUIRED_FULL_JUMP_PRESSING_DURATION;
  _isOnGround = false;
  _halfJumpMaxY = _y + SHORT_JUMP_HEIGHT;
}

void Player::takeDamage(float damage) {
  if (_timeToLiveWithoutHealth > 0) return;
  _timeToLiveWithoutHealth = LIFE_WITHOUT_HEALTH_DURATION;
  _speedX *= HIT_SPEED_COEFFICIENT;
  _speedY += HIT_ACCELERATION_Y;
  _health -= damage;
}

void Player::shoot() {
  if (!_isControlable) return;
  if (_weaponCooldown <= 0) {
    _world->addMissle(
      missleInititalX(),
      _y + missleInitialY(),
      MISSLE_SPEED * _dirrection,
      0,
      &_missleType
    );
    _weaponCooldown = WEAPON_COOLDOWN;
  }
}

void Player::onTileCollision(WorldVector correction) {
  if (correction.x != 0) {
    _x += _moveVector.x + correction.x;
    _speedX = 0;
  }
  if (correction.y != 0) {
    _y += _moveVector.y + correction.y;
    _speedY = 0;
    if (correction.y > 0) _isOnGround = true;
  }
  
}

float Player::speedX() {
  return _speedX;
}

inline float decellerate(const float &speed, const float &decelleration) {
  return copysignf(std::max<float>(0, abs(speed) - decelleration), speed);
}

void Player::update(float timePassed) {
  _moveVector.x = _speedX * timePassed;
  _moveVector.y = _speedY * timePassed;

  _x += _moveVector.x;
  _y += _moveVector.y;

  if (abs(_speedY) > 1) _isOnGround = false;
  if (&getCurrentAnimation() == &_deathAnimation) {
    _isControlable = false;
    return;
  }
  _world->hit(_x, _y, _x + _width, _y + _height, true);
  if (_timeToLiveWithoutHealth > 0) _timeToLiveWithoutHealth -= timePassed;
  if (_weaponCooldown > 0) _weaponCooldown -= timePassed;

  if ((_y > _halfJumpMaxY) && _jumpBeingPressedDuration > 0) {
    _speedY = 0;
    _jumpBeingPressedDuration = 0;
  }
  _speedY -= GRAVITY_ACCELERATION_Y * timePassed;
  if (_isOnGround) _speedX = decellerate(_speedX, DRAG_DECELLERATION_X * timePassed);

  _hitDamage = _world->hit(
    _x - 0.5, _y, _x + _width + 0.5,
    _y + _height + 0.5, true
  );
  if (_hitDamage != 0) takeDamage(_hitDamage);

  if (_health > 0) return;
  if (_isOnGround) {
    _isControlable = false;
    _moveVector.y = 0;
    return;
  }
  _timeToLiveWithoutHealth = LIFE_WITHOUT_HEALTH_AFTER_LANDING_DURATION * timePassed;
}

void Player::drawGizmo()  // debug option. Displays collision box.
{
  DrawRectangle(floor(_x), floor(_y), ceil(_height), ceil(_height), YELLOW);
}

Animation& Player::getCurrentAnimation() {
  if (_isOnGround) {
    if (_health <= 0) return _deathAnimation;
    return abs(_speedX) > WALKING_ANIMATION_SPEED_THRESHOLD
      ? _walkAnimation
      : _standAnimation;
  }
  return _speedY > 0 ? _jumpAnimation : _fallAnimation;
}

void Player::draw() {
  float x1, x2;
  if (_dirrection < 0) {
    x1 = _x;
    x2 = _x + 2;
  } else {
    x2 = _x + _width;
    x1 = x2 - 2;
  };
  Animation& currentAnimation = getCurrentAnimation();
  if (&currentAnimation != &_deathAnimation) {
    currentAnimation.setCol(_currentAnimationFrameIndex);
    // Preserving wheel position between animations

    _currentAnimationFrameIndex = currentAnimation.draw(-_dirrection, x1, _y, x2, _y + 2);
    currentAnimation.stop();
  } else
    currentAnimation.draw(-1, x1 - 0.5, _y, x1 + 3.5, _y + 4);
}

Player::Player(float a_x, float a_y, TextureKeeper a_texture,
               TextureKeeper a_missleTexture, IWorld &a_world) {
  _x = a_x;
  _y = a_y;
  _dirrection = 1;
  _hitDamage = 0;
  _halfJumpMaxY = _y + SHORT_JUMP_HEIGHT;
  _jumpBeingPressedDuration = 0;
  _world = &a_world;
  _speedX = 0;
  _speedY = 0;
  _width = WIDTH;
  _height = HEIGHT;
  _isControlable = true;

  // Animation
  _texture = a_texture;
  _missleTexture = a_missleTexture;
  _standAnimation = Animation(_texture, ANIMATION_FRAME_SIZE, 0, 0, 1, 4, 1, LOOP);
  _walkAnimation = Animation(_texture, ANIMATION_FRAME_SIZE, 0, 0, 3, 4, 3, LOOP);
  _jumpAnimation = Animation(_texture, ANIMATION_FRAME_SIZE, 0, 3, 1, 4, 2, LOOP);
  _fallAnimation = Animation(_texture, ANIMATION_FRAME_SIZE, 0, 2, 1, 4, 2, LOOP);
  _deathAnimation = Animation(_texture, DEATH_ANIMATION_FRAME_SIZE, 0, 2, 1, 3, 3, ONCE);
  _currentAnimationFrameIndex = 0;

  // Weapon
  _missleType = {
    /*burstAnim*/ Animation(a_missleTexture, 0.25f, 3, 0, 4, 1, 2, ONCE),
    /*flyAnim*/ Animation(a_missleTexture, 0.5f, 0.25f, 0, 0, 1, 1, 0, LOOP),
    /*spriteX*/ -1.5,
    /*spriteY*/ -0.4,
    /*foe*/ false,
    /*damage*/ 1.5,
    /*texture*/ a_missleTexture,
    /*falling*/ false,
  };
}
