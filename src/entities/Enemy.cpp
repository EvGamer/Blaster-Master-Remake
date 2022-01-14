#include "Enemy.h"

namespace {
  const int RELOAD_TIME = 60;
}

Enemy::Enemy(float a_x, float a_y, char a_dirrection, TextureKeeper a_texture,
             IWorld *a_world) {
  _weaponReloadCooldown = 0;
  _weaponCooldown = 0;
  _texture = a_texture;
  _dirrection = a_dirrection;
  _health = 6;
  _speedY = 0;
  _speedX = SPEED_X;
  _isHit = false;
  _x = a_x;
  _y = a_y;
  _width = 1.6;
  _height = 1.1;
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
      /*foe*/ true,
      /*damage*/ 1,
      /*texture*/ _texture,
      /*falling*/ true,
  };
}

inline void Enemy::kill() {
  if (_currentAnimation != &_deathAnimation) {
    _health = 0;
    _currentAnimation = &_deathAnimation;
    _speedX = 0;
    _y -= 0.5;
    _dirrection = -1;
  }
}

void Enemy::update(Player &player) {
  if (_health > 0) {
    if (_speedY <= 0) {
      if (_world->isSolidTileAtCoord(_x + 0.05, _y) ||
          _world->isSolidTileAtCoord(_x + _width - 0.05, _y))
        _y = floor(_y + 1);
    }
    if (_weaponReloadCooldown > 0) _weaponReloadCooldown--;
    if (_weaponMagazine > 0) {
      _speedX = 0;
      _currentAnimation = &_shootAnimation;
      if (_weaponCooldown <= 0) {
        _weaponCooldown = 10;
        _world->addMissle(_x + _width * 0.5, _y + _height - 0.4,
                           0.4 * _dirrection, 0.4f, &_missleType);
        _weaponMagazine--;
      } else
        _weaponCooldown--;

    } else {
      _speedX = 1;
      _currentAnimation = &_walkAnimation;
    }
    _x += 0.05 * _speedX * _dirrection;

    float damage = _world->hit(_x, _y, _x + _width, _y + _height, false);
    if (damage > 0) _isHit = true;
    _health -= damage;

    const float frontX = _dirrection < 0 ? _x : _x + _width;
    bool shouldFlip = 
      (_dirrection != 0) 
      && (_world->isSolidTileAtCoord(frontX, _y) || !_world->isSolidTileAtCoord(frontX, _y - 1));
    
    if (shouldFlip)
      _dirrection = -_dirrection;

    if (_health <= 0) kill();
    float y1 = _y + _height * 0.5;

    if (player.isPointWithin(_x + 0.1, y1) || player.isPointWithin(_x + _width - 0.1, y1))
      player.takeDamage(0.7f);
  
    if ((floor(player.getY()) == floor(_y)) &&
        (floor(player.getMissleInititalX() - _x) == 3 * _dirrection) && (_weaponReloadCooldown <= 0)) {
      _weaponReloadCooldown = RELOAD_TIME;
      _weaponMagazine = 4;
    }
  }
}

void Enemy::draw() {
  float x1, x2;
  if (_dirrection > 0) {
    x2 = _x + _width;
    x1 = x2 - 2;
  } else {
    x1 = _x;
    x2 = x1 + 2;
  }
  _currentAnimation->draw(-_dirrection, x1, _y, x2, _y + 2);
  _isHit = false;
}