#include "Enemy.h"

const int RELOAD_TIME = 60;

Enemy::Enemy(float a_x, float a_y, char a_dirrection, GLuint a_textureId,
             IWorld *a_world) {
  _weaponReloadCooldown = 0;
  _weaponCooldown = 0;
  _textureId = a_textureId;
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
  _deathAnimation = Animation(_textureId, 0.25, 0, 2, 1, 4, 2, ONCE);
  _walkAnimation = Animation(_textureId, 0.25, 0, 0, 1, 4, 5, LOOP);
  _shootAnimation = Animation(_textureId, 0.25, 0, 1, 1, 4, 3, ONCE);
  _currentAnimation = &_walkAnimation;
  _missleType = {
      /*burstAnim*/ Animation(_textureId, 0.125, 0, 7, 1, 4, 2, ONCE),
      /*flyAnim*/ Animation(_textureId, 0.25, 0.125, 0, 6, 1, 1, 2, ONCE),
      /*spriteX*/ -0.75,
      /*spriteY*/ -0.5,
      /*foe*/ true,
      /*damage*/ 1,
      /*textureId*/ _textureId,
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

void Enemy::update(Player &p1) {
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
    float hurt = _world->hit(_x, _y, _x + _width, _y + _height, false);
    if (hurt > 0) _isHit = true;
    _health -= hurt;
    if (((_dirrection > 0) && (_world->isSolidTileAtCoord(_x + _width, _y) ||
                         !_world->isSolidTileAtCoord(_x + _width, _y - 1))) ||
        ((_dirrection < 0) &&
         (_world->isSolidTileAtCoord(_x, _y) || !_world->isSolidTileAtCoord(_x, _y - 1))))
      _dirrection = -_dirrection;
    if (_health <= 0) kill();
    float y1 = _y + _height * 0.5;

    if (p1.isPointWithin(_x + 0.1, y1) || p1.isPointWithin(_x + _width - 0.1, y1))
      p1.hurt(0.7f);
    if ((floor(p1.getY()) == floor(_y)) &&
        (floor(p1.getMissleInititalX() - _x) == 3 * _dirrection) && (_weaponReloadCooldown <= 0)) {
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
  if (_isHit) glColor3f(1, 0.5, 0);
  _currentAnimation->draw(-_dirrection, x1, _y, x2, _y + 2);
  _isHit = false;
}