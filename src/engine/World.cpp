
#include <vector>
#include "World.h"

World::World(String filename) {
  _map = Map(filename);

  _gravity = 1;
  _friction = 0;
  _playerTextureId = loadTexture("Sprites\\SOPHIA.tga");
  _playerMissleTextureId = loadTexture("Sprites\\Shot.tga");
  _enemyTextureId = loadTexture("Sprites\\enemy.tga");
}

World::~World() {
}

void World::init() {
  enemies = std::list<Enemy>();
  player = nullptr;

  for (EntityDescription entity : _map.entities) {
    Point pos = _map.pixelToTileCoord(entity.pixelCoord);
    if (entity.type == "Player") {
      if (player == nullptr) {
        player = new Player(
          pos.x, pos.y,
          _playerTextureId,
          _playerMissleTextureId,
          *this
        );
      }
    }
    else if (entity.type == "Enemy") {
      enemies.emplace_back(
        pos.x, pos.y,
        entity.isFacingRight ? 1 : -1,
        _enemyTextureId,
        this
      );
    }
  }
}

void World::addMissle(float x, float y, float speedX, float speedY,
                    MissleTraits *wpn) {
  _missles.push_back({
      wpn->burstAnim,
      wpn->flyAnim,
      wpn->spriteX,
      wpn->spriteY,
      x,
      y,
      speedX,
      speedY,
      false,
      wpn->foe,
      wpn->falling,
      wpn->damage,
  });
};

float World::hit(float x1, float y1, float x2, float y2, bool foe) {
  float damage = 0;

  for (auto &missle : _missles) {
    bool isInsideWidth = (missle.x >= x1) && (missle.x <= x2);
    bool isInsideHeight = (missle.y >= y1) && (missle.y <= y2);
    bool isInside = isInsideWidth && isInsideHeight;

    if (isInside && (foe == missle.foe) && (!missle.hit)) {
      if (missle.damage > damage) damage = missle.damage;
      missle.hit = true;
      missle.burstAnim.freeze();
    }
  }
  return damage;
};

bool World::isSolidTileAtCoord(float x, float y) {
  if (_map.coordOutOfRange(x, y)) return false;
  return !_map.coordOutOfRange(x, y) && _map.getTileTraits(floor(x), floor(y)).isSolid;
};

bool World::getFrict(unsigned char i, unsigned char j) {
  return !_map.coordOutOfRange(i, j) && _map.getTileTraits(i, j).isSolid ? _friction : 0;
}

bool isMissleExploded(Missle &value) {
  return value.burstAnim.isEnded();
}

void World::updateMissles() {
  for (auto& missle : _missles) {
    if (!missle.hit) {
      missle.x += missle.speedX;
      missle.y += missle.speedY;
      if (missle.falling) missle.speedY -= 0.1 * _gravity;
      missle.hit = isSolidTileAtCoord(missle.x, missle.y);
    }
  }
  _missles.remove_if(isMissleExploded);
}

bool World::isPlayerInRoom(Room& room) {
  return room.id != 0 && room.area.isContainRectangle(player->getRect());
};

void World::updateCamera() {
  if (player == nullptr) return;
  _cameraX = player->getX();
  _cameraY = player->getY();
  Rect& room = _currentRoom.area;

  if (room.width < _halfScreenWidth * 2) {
    _cameraX = room.getCenterX();
  }
  else if (_cameraX - _halfScreenWidth < room.getWest()) {
    _cameraX = room.getWest() + _halfScreenWidth;
  }
  else if (_cameraX + _halfScreenWidth > room.getEast()) {
    _cameraX = room.getEast() - _halfScreenWidth;
  }

  if (room.height < _halfScreenHeight * 2) {
    _cameraY = room.getCenterY();
  }
  else if (_cameraY - _halfScreenHeight < room.getSouth()) {
    _cameraY = room.getSouth() + _halfScreenHeight;
  }
  else if (_cameraY + _halfScreenHeight > room.getNorth()) {
    _cameraY = room.getNorth() - _halfScreenHeight;
  }
}

void World::applyCamera() {
  glLoadIdentity();
  glTranslatef(
    -_cameraX / _halfScreenWidth + 1,
    -_cameraY / _halfScreenHeight + 1,
    0
  );
}

void World::updateCurrentRoom() {
  if (isPlayerInRoom(_currentRoom)) return;

  for (Room& room : _map.rooms) {
    if (room.id == _currentRoom.id) continue;
    if (isPlayerInRoom(room)) {
      _currentRoom = room;
      return;
    }
  }
}

void World::detectTileCollision(Entity& entity) {
  Point correction{0, 0};
  UInt minTileX = max(0, floor(entity.getWest()));
  UInt maxTileX = max(0, floor(entity.getEast()));
  UInt minTileY = max(0, floor(entity.getSouth()));
  UInt maxTileY = max(0, floor(entity.getNorth()));
  bool isBottomLeftSolid = _map.getTileTraits(minTileX, minTileY).isSolid;
  bool isBottomRightSolid = _map.getTileTraits(maxTileX, minTileY).isSolid;
  bool isTopLeftSolid = _map.getTileTraits(minTileX, maxTileY).isSolid;
  bool isTopRightSolid = _map.getTileTraits(maxTileX, maxTileY).isSolid;
  float bottomPush = minTileY - entity.getSouth() + 1;
  float topPush = maxTileY - entity.getNorth();
  float leftPush = minTileX - entity.getWest() + 1;
  float rightPush = maxTileX - entity.getEast();

  float speedY = entity.getSpeedY();
  if ((entity.getSpeedY() < 0) && (isBottomRightSolid && isBottomLeftSolid)) {
    correction.y = bottomPush;
  } 
  else if ((entity.getSpeedY() > 0) && (isTopRightSolid && isTopLeftSolid)) {
    correction.y = topPush;
  }

  float speedX = entity.getSpeedX();
  if ((entity.getSpeedX() < 0) && (isBottomLeftSolid && isTopLeftSolid)) {
    correction.x = leftPush;
  }
  else if ((entity.getSpeedX() > 0) && (isBottomRightSolid && isTopRightSolid)) {
    correction.x = rightPush;
  }
  if (correction.x != 0 && correction.y != 0) {
    entity.onTileCollision(correction);
    return;
  };
}

void World::update() {
  if (player != nullptr) {
    player->updatePosition();
    detectTileCollision(*player);
    player->update();
  }
  updateCamera();
  updateMissles();
  updateCurrentRoom();
}

void World::drawMap() {
  UInt tX0 = max(max(floor(_cameraX - _halfScreenWidth), 0), ceil(_currentRoom.area.getWest()));
  UInt tY0 = max(max(floor(_cameraY - _halfScreenHeight), 0), ceil(_currentRoom.area.getSouth()));
  UInt tXn = min(max(ceil(_cameraX + _halfScreenWidth), 0), floor(_currentRoom.area.getEast()));
  UInt tYn = min(max(ceil(_cameraY + _halfScreenHeight), 0), floor(_currentRoom.area.getNorth()));
  for (UInt i = tX0; i < tXn; i++) {
    for (UInt j = tY0; j < tYn; j++) {
      _map.drawTile(i, j);
    };
  };
}

void World::drawMissles() {
  char dir;
  float x0;
  float x1, x2;
  float y0;
  for (auto& missle : _missles) {
    if (missle.speedX < 0) {
      dir = -1;
      x1 = missle.x - missle.spriteX;
      x0 = x1 - 2;
      x2 = x0;
    }
    else {
      dir = 1;
      x0 = missle.x + missle.spriteX;
      x1 = x0 + 2;
      x2 = x1 - 1;
    }
    y0 = missle.y;
    if (!missle.hit)
      missle.flyAnim.draw(dir, x0, y0, x1, y0 + 1);
    else
      missle.burstAnim.draw(1, x2, y0, x2 + 1, y0 + 1);
  }
}

void World::draw() {
  drawMap();
  player->draw();
  drawMissles();
}
