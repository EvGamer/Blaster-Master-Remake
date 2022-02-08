#include "Enemy.h"

namespace {
  const int RELOAD_TIME = 60;
  const float WIDTH = 1.6;
  const float HEIGHT = 1.1;
  const int HEALTH = 6;
  const int WEAPON_COOLDOWN = 10;
  const float TOUCH_DAMAGE = 0.7f;
  const int AGRO_RANGE = 3;
  const int WEAPON_MAGAZINE = 4;
  const float PLAYER_HITBOX_REDUCTION = 0.1;
  const float MISSLE_INITIAL_SPEED_X = 0.4;
  const float MISSLE_INITIAL_SPEED_Y = 0.4;
  const float SPEED_X = 0.05;
  const float SPRITE_SHIFT_X = 2;
  const float DEATH_SPRITE_SHIFT = 0.5;
}

Enemy::Enemy(
  float a_x, float a_y, char a_dirrection, TextureResource a_texture, IWorld *a_world
) {
  _weaponReloadCooldown = 0;
  _weaponCooldown = 0;
  _texture = a_texture;
  _dirrection = a_dirrection;
  _health = HEALTH;
  _speedY = 0;
  _speedX = SPEED_X;
  _isHit = false;
  _x = a_x;
  _y = a_y;
  _width = WIDTH;
  _height = HEIGHT;
  _world = a_world;
  _deathAnimation = Animation(_texture, 0.25, 0, 2, 1, 4, 2, ONCE);
  _walkAnimation = Animation(_texture, 0.25, 0, 0, 1, 4, 5, LOOP);
  _shootAnimation = Animation(_texture, 0.25, 0, 1, 1, 4, 3, ONCE);
  _currentAnimation = &_walkAnimation;
  _missleType = {
    /*burstAnim*/ Animation(_texture, 0.125, 0, 7, 1, 4, 2, ONCE),
    /*flyAnim*/ Animation(_texture, 0.25, 0.125, 0, 6, 1, 1, 2, ONCE),
    /*spriteX*/ -0.75,
    /*spriteY*/ -0.5,
    /*isFromEnemy*/ true,
    /*damage*/ 1,
    /*texture*/ _texture,
    /*canFall*/ true,
  };
}

inline void Enemy::kill() {
  if (_currentAnimation == &_deathAnimation) return;
  _health = 0;
  _currentAnimation = &_deathAnimation;
  _speedX = 0;
  _y -= DEATH_SPRITE_SHIFT;
  _dirrection = FACING_LEFT;
}

void Enemy::update(Player &player) {
  if (_health <= 0) return;

  const bool didHitFloor = 
    _speedY <= 0
    && (
      _world->isSolidTileAtCoord(_x + 0.05, _y) 
      || _world->isSolidTileAtCoord(_x + _width - 0.05, _y)
    );

  if (didHitFloor)
    _y = floor(_y + 1);
  if (_weaponReloadCooldown > 0)
    _weaponReloadCooldown--;

  if (_weaponMagazine > 0) {
    _speedX = 0;
    _currentAnimation = &_shootAnimation;
    if (_weaponCooldown <= 0) {
      _weaponCooldown = WEAPON_COOLDOWN;
      _world->addProjectile(
        _x + _width * 0.5, _y + _height - 0.4,
        MISSLE_INITIAL_SPEED_X * _dirrection,
        MISSLE_INITIAL_SPEED_Y,
        &_missleType
      );
      _weaponMagazine--;
    } else
      _weaponCooldown--;

  } else {
    _speedX = SPEED_X;
    _currentAnimation = &_walkAnimation;
  }
  _x += _speedX * _dirrection;

  float damage = _world->checkProjectileCollision(_x, _y, _x + _width, _y + _height, false);
  if (damage > 0) 
    _isHit = true;
  _health -= damage;

  const float frontX = _dirrection < 0 ? _x : _x + _width;
  bool shouldFlip = 
    (_dirrection != 0) 
    && (
      _world->isSolidTileAtCoord(frontX, _y) 
      || !_world->isSolidTileAtCoord(frontX, _y - 1)
    );
  
  if (shouldFlip)
    _dirrection = -_dirrection;

  if (_health <= 0) kill();
  float y1 = _y + _height * 0.5;

  const bool didHitPlayer = 
    player.isPointWithin(_x + PLAYER_HITBOX_REDUCTION, y1)
    || player.isPointWithin(_x + _width - PLAYER_HITBOX_REDUCTION, y1);
  
  if (didHitPlayer)
    player.takeDamage(TOUCH_DAMAGE);

  bool shouldStartShooting = 
    (floor(player.y()) == floor(_y)) 
    && (floor(player.missleInititalX() - _x) == AGRO_RANGE * _dirrection)
    && (_weaponReloadCooldown <= 0);
  
  if (shouldStartShooting) {
    _weaponReloadCooldown = RELOAD_TIME;
    _weaponMagazine = WEAPON_MAGAZINE;
  }
}

void Enemy::draw() {
  float left, right;
  if (_dirrection > 0) {
    right = _x + _width;
    left = right - SPRITE_SHIFT_X;
  } else {
    left = _x;
    right = left + SPRITE_SHIFT_X;
  }
  _currentAnimation->draw(-_dirrection, left, _y, right, _y + SPRITE_SHIFT_X);
  _isHit = false;
}