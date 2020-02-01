#include "Entity.h"

void Entity::updatePosition() {
  _x = _x + _speedX;
  _y = _y + _speedY;
}
