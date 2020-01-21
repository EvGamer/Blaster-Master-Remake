#include "Player.h"

using namespace PlayerConstants;

void Player::move(int dirrection) {
  if (_isControlable) {
    _dirrection = dirrection;
    if (_speedX < 1) _speedX += 0.1;
    if (_currentAnimation == &_standAnimation) {
      _currentAnimation = &_walkAnimation;
    }
    _currentAnimation->unfreeze();
  }
}

void Player::jump() {
  if (_isControlable) {
    _jumpBeingPressedDuration--;
    if (_isOnGround) {
      _speedY = _maxJumpHeight;
      _jumpBeingPressedDuration = 20;
      _isOnGround = false;
      _halfJumpMaxY = _y + JUMP_HEIGHT;
      _currentAnimation = &_jumpAnimation;
      _missleInitialY = CANNON_Y;
    } else {
      _jumpBeingPressedDuration--;
    }
  }
}

void Player::hurt(float damage) {
  if (_timeToLiveWithoutHealth <= 0) {
    _timeToLiveWithoutHealth = 45;
    _speedX *= 0.5;
    _speedY += 1;
    _health -= damage;
    damage = 0;
  }
}

void Player::shoot() {  // if(_isControlable)
  if (_weaponCooldown <= 0) {
    _world->addMissle(_missleInitialX, _y + _missleInitialY, 0.3 * _dirrection, 0, &_missleType);
    _weaponCooldown = 15;
  }
}

void Player::continueFalling() {
  if (_world->collide(_x + 0.05, _y) ||
      _world->collide(_x + _width - 0.05, _y)) {
    _y = floor(_y + 1);
    _speedY = 0;
    _isOnGround = true;
    _currentAnimation = &_standAnimation;
    _missleInitialY = CANNON_Y_FALLING;
  } else {
    _isOnGround = false;
    _currentAnimation = &_fallAnimation;
  }
}

void Player::update() {
  if (_currentAnimation != &_deathAnimation) {
    _world->hit(_x, _y, _x + _width, _y + _height, true);
    if (_timeToLiveWithoutHealth > 0) _timeToLiveWithoutHealth--;
    if (_weaponCooldown > 0) _weaponCooldown--;
    _y += _jumpSpeed * _speedY;

    if ((_y > _halfJumpMaxY) && _jumpBeingPressedDuration > 0) {
      _speedY = 0;
      _jumpBeingPressedDuration = 0;
    }
    if (_speedY <= 0) continueFalling();

    _x += _dirrection * _speedX * 0.11;
    // if
    // (!(_world->collide(_x,_y-1)&&_world->collide(_x+_width,_y-1))){_isOnGround=false;}
    _speedY -= _jumpSpeed * _maxJumpHeight * 0.5 / _world->getGravity();
    _speedX -= 0.05;

    if (_speedX < 0) {
      _speedX = 0;
    };
    if (_dirrection > 0) {
      if (_world->collide(_x + _width, _y)) {
        _x = floor(_x + 2) - _width;
        _speedX = 0;
      }
      _missleInitialX = _x + _width;
    } else {
      if (_world->collide(_x, _y)) {
        _x = floor(_x + 1);
        _speedX = 0;
      }
      _missleInitialX = _x;
    }

    if (_speedY > 0) {
      if ((_world->collide(_x, _y + _height)) ||
          (_world->collide(_x + _width - 0.1f, _y + _height)))  //
      {
        _y = floor(_y + _height) - _height;
        _speedY = 0;
      };
    };

    if ((_speedX > 0.3) && _isOnGround) {
      _currentAnimation = &_walkAnimation;
      _currentAnimation->unfreeze();
    }

    _hitDamage = _world->hit(_x - 0.5, _y, _x + _width + 0.5,
                               _y + _height + 0.5, true);
    if (_hitDamage != 0) hurt(_hitDamage);
    if (_health <= 0) {
      if (_isOnGround) {
        _currentAnimation = &_deathAnimation;
        _isControlable = false;
        _speedX = 0;
      } else {
        _timeToLiveWithoutHealth = 2;
      }
    }
  }
}

void Player::revive() {
  _deathAnimation.unfreeze();
  _currentAnimation = &_standAnimation;
  _health = 8;
  _x = _initialX;
  _y = _initialY;
  _isControlable = true;
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

void Player::draw() {
  float x1, x2;
  if (_dirrection < 0) {
    x1 = _x;
    x2 = _x + 2;
  } else {
    x2 = _x + _width;
    x1 = x2 - 2;
  };
  if (_currentAnimation != &_deathAnimation) {
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

    _currentAnimation->setCol(_currentAnimationFrameIndex);
    // Preserving wheel position between animations

    _currentAnimationFrameIndex = _currentAnimation->draw(-_dirrection, x1, _y, x2, _y + 2);
    _currentAnimation->freeze();
  } else
    _currentAnimation->draw(-1, x1 - 0.5, _y, x1 + 3.5, _y + 4);
}

Player::Player(float a_x, float a_y, GLuint a_textureId,
               GLuint a_missleTextureId, World *a_world) {
  _x = a_x;
  _y = a_y;
  _dirrection = 1;
  _missleInitialY = CANNON_Y;
  _missleInitialX = _x;
  _hitDamage = 0;
  _halfJumpMaxY = _y + JUMP_HEIGHT;
  _jumpBeingPressedDuration = 0;
  _world = a_world;
  _initialX = _x;
  _initialY = _y;
  _speedX = 0;
  _speedY = 0;
  _width = 26.0 / 16.0;
  _height = 18.0 / 16.0;
  _isControlable = true;

  // Animation
  _textureId = a_textureId;
  _missleTextureId = a_missleTextureId;
  _standAnimation = Animation(_textureId, 0.125, 0, 0, 1, 4, 1, LOOP);
  _walkAnimation = Animation(_textureId, 0.125, 0, 0, 3, 4, 3, LOOP);
  _jumpAnimation = Animation(_textureId, 0.125, 0, 3, 1, 4, 2, LOOP);
  _fallAnimation = Animation(_textureId, 0.125, 0, 2, 1, 4, 2, LOOP);
  _deathAnimation = Animation(_textureId, 0.25, 0, 2, 1, 3, 3, ONCE);
  _currentAnimation = &_standAnimation;
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
