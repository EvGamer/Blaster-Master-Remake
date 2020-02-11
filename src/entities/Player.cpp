#include "Player.h"

using namespace PlayerConstants;

void Player::move(int dirrection) {
  if (_isControlable) {
    _dirrection = dirrection;
    if (abs(_speedX) < MAX_SPEED_X) _speedX += copysignf(WALK_ACCELERATION, _dirrection);
  }
  // wheels should only turn then player presses the button
  getCurrentAnimation().unfreeze();
}

void Player::jump() {
  if (_isControlable) {
    _jumpBeingPressedDuration--;
    if (_isOnGround) {
      _speedY = INITIAL_JUMP_SPEED_Y;
      _jumpBeingPressedDuration = REQUIRED_FULL_JUMP_PRESSING_DURATION;
      _isOnGround = false;
      _halfJumpMaxY = _y + JUMP_HEIGHT;
    } else {
      _jumpBeingPressedDuration--;
    }
  }
}

void Player::hurt(float damage) {
  if (_timeToLiveWithoutHealth <= 0) {
    _timeToLiveWithoutHealth = LIFE_WITHOUT_HEALTH_DURATION;
    _speedX *= HIT_SPEED_COEFFICIENT;
    _speedY += HIT_ACCELERATION_Y;
    _health -= damage;
    damage = 0;
  }
}

void Player::shoot() {  // if(_isControlable)
  if (_weaponCooldown <= 0) {
    _world->addMissle(
      getMissleInititalX(),
      _y + getMissleInitialY(),
      MISSLE_SPEED * _dirrection,
      0,
      &_missleType
    );
    _weaponCooldown = WEAPON_COOLDOWN;
  }
}

void Player::onTileCollision(Point correction) {
  _x += _speedX + correction.x;
  _y += _speedY + correction.y;
  if (correction.x != 0) _speedX = 0;
  if (correction.y != 0) _speedY = 0;
  _isOnGround = correction.y > 0;
}

float Player::getSpeedX() {
  return _speedX;
}

inline float decellerate(const float &speed, const float &decelleration) {
  return copysignf(max(0, abs(speed) - decelleration), speed);
}

void Player::update() {
  _x += _speedX;
  _y += _speedY;
  if (&getCurrentAnimation() != &_deathAnimation) {
    _world->hit(_x, _y, _x + _width, _y + _height, true);
    if (_timeToLiveWithoutHealth > 0) _timeToLiveWithoutHealth--;
    if (_weaponCooldown > 0) _weaponCooldown--;

    if ((_y > _halfJumpMaxY) && _jumpBeingPressedDuration > 0) {
      _speedY = 0;
      _jumpBeingPressedDuration = 0;
    }
    _speedY -= GRAVITY_ACCELERATION_Y;
    _accelerationY = max(0, _accelerationY - JUMP_ACCELERATION_LOSS);
    if (_isOnGround) _speedX = decellerate(_speedX, DRAG_DECELLERATION_X);

    _hitDamage = _world->hit(_x - 0.5, _y, _x + _width + 0.5,
                               _y + _height + 0.5, true);
    if (_hitDamage != 0) hurt(_hitDamage);
    if (_health <= 0) {
      if (_isOnGround) {
        _isControlable = false;
        _speedX = 0;
      } else {
        _timeToLiveWithoutHealth = LIFE_WITHOUT_HEALTH_AFTER_LANDING_DURATION;
      }
    }
  }
}

void Player::drawGizmo()  // debug option. Displays collision box.
{
  glColor3f(0, 1, 1);
  glBegin(GL_QUADS);
  glVertex2f(floor(_x), floor(_y));
  glVertex2f(floor(_x), ceil(_y + _height));
  glVertex2f(ceil(_x + _width), ceil(_y + _height));
  glVertex2f(ceil(_x + _width), floor(_y));
  glEnd();
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
    switch (_timeToLiveWithoutHealth % 3) {
      case 0:
        glColor3f(1, 1, 1);
        break;
      case 1:
        glColor3f(0.5, 1, 0.7);
        break;
      case 2:
        glColor3f(1, 0.3, 0);
        break;
    }

    currentAnimation.setCol(_currentAnimationFrameIndex);
    // Preserving wheel position between animations

    _currentAnimationFrameIndex = currentAnimation.draw(-_dirrection, x1, _y, x2, _y + 2);
    currentAnimation.freeze();
  } else
    currentAnimation.draw(-1, x1 - 0.5, _y, x1 + 3.5, _y + 4);
}

Player::Player(float a_x, float a_y, GLuint a_textureId,
               GLuint a_missleTextureId, IWorld &a_world) {
  _x = a_x;
  _y = a_y;
  _dirrection = 1;
  _hitDamage = 0;
  _halfJumpMaxY = _y + JUMP_HEIGHT;
  _jumpBeingPressedDuration = 0;
  _world = &a_world;
  _speedX = 0;
  _speedY = 0;
  _width = WIDTH;
  _height = HEIGHT;
  _isControlable = true;

  // Animation
  _textureId = a_textureId;
  _missleTextureId = a_missleTextureId;
  _standAnimation = Animation(_textureId, ANIMATION_FRAME_SIZE, 0, 0, 1, 4, 1, LOOP);
  _walkAnimation = Animation(_textureId, ANIMATION_FRAME_SIZE, 0, 0, 3, 4, 3, LOOP);
  _jumpAnimation = Animation(_textureId, ANIMATION_FRAME_SIZE, 0, 3, 1, 4, 2, LOOP);
  _fallAnimation = Animation(_textureId, ANIMATION_FRAME_SIZE, 0, 2, 1, 4, 2, LOOP);
  _deathAnimation = Animation(_textureId, DEATH_ANIMATION_FRAME_SIZE, 0, 2, 1, 3, 3, ONCE);
  _currentAnimationFrameIndex = 0;

  // Weapon
  _missleType = {
    /*burstAnim*/ Animation(a_missleTextureId, 0.25f, 3, 0, 4, 1, 2, ONCE),
    /*flyAnim*/ Animation(a_missleTextureId, 0.5f, 0.25f, 0, 0, 1, 1, 0, LOOP),
    /*spriteX*/ -1.5,
    /*spriteY*/ -0.4,
    /*foe*/ false,
    /*damage*/ 1.5,
    /*textureId*/ a_missleTextureId,
    /*falling*/ false,
  };
}
