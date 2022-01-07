#include <vector>
#include "World.h"
#include <memory>


World::World() {
  _gravity = 0.01;
  _friction = 0;
}

void World::loadTextures(WorldTextureFileNames fileNames) {
  _playerTexture = TextureKeeper(fileNames.player);
  _playerMissleTexture = TextureKeeper(fileNames.playerMissle);
  _enemyTexture = TextureKeeper(fileNames.enemy);
  _map = Map(fileNames.area);
}

void World::init() {
  enemies = std::list<Enemy>();
  player.reset();

  for (EntityDescription entity : _map.entities) {
    Point pos = _map.pixelToTileCoord(entity.pixelCoord);
    if (entity.type == "Player" && !player) {
      player = std::make_unique<Player>(
        pos.x, pos.y,
        _playerTexture,
        _playerMissleTexture,
        *this
      );
    } else if (entity.type == "Enemy") {
      enemies.emplace_back(
        pos.x, pos.y,
        entity.isFacingRight ? 1 : -1,
        _enemyTexture,
        this
      );
    }
  }
}

void World::addMissle(
  float x, float y, float speedX, float speedY,
  MissleTraits *wpn
) {
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
      if (missle.falling) missle.speedY -= 0.1;
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
  else if (_cameraX - _halfScreenWidth < room.getLeft()) {
    _cameraX = room.getLeft() + _halfScreenWidth;
  }
  else if (_cameraX + _halfScreenWidth > room.getRight()) {
    _cameraX = room.getRight() - _halfScreenWidth;
  }

  if (room.height < _halfScreenHeight * 2) {
    _cameraY = room.getCenterY();
  }
  else if (_cameraY - _halfScreenHeight < room.getBottom()) {
    _cameraY = room.getBottom() + _halfScreenHeight;
  }
  else if (_cameraY + _halfScreenHeight > room.getTop()) {
    _cameraY = room.getTop() - _halfScreenHeight;
  }
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

Point* getCorrectionFromOverlap(const Rect& overlap, bool shouldPushVertical, float dx, float dy) {
  return shouldPushVertical 
    ? new Point{ 0, copysignf(overlap.height, -dy) }
    : new Point{ copysignf(overlap.width, -dx), 0 };
}

Point* World::_getSingleTileCollision(Rect &entity, UInt tileX, UInt tileY, float dx, float dy) {
  if (!_map.getTileTraits(tileX, tileY).isSolid) return nullptr;
  Rect tile(tileX, tileY, 1, 1);
  Rect overlap = entity.getOverlap(tile);
  return overlap.getTop() == tile.getTop()
    ? getCorrectionFromOverlap(overlap, dy < 0, dx, dy)
    : getCorrectionFromOverlap(overlap, dy > 0, dx, dy);
}

void World::detectTileCollision(Entity& entity) {
  Point correction{0, 0};
  Rect box = entity.getRect();
  Rect newBox = entity.getRect();
  float dx = entity.getSpeedX();
  float dy = entity.getSpeedY();
  newBox.x += dx;
  newBox.y += dy;
  using std::max;
  UInt tileXLeft = max<UInt>(0, floor(newBox.x));
  UInt tileYBottom = max<UInt>(0, floor(newBox.y));
  UInt tileXRight = max<UInt>(0, floor(newBox.getRight()));
  UInt tileYTop = max<UInt>(0, floor(newBox.getTop()));
  bool isSolidLeftBottom = _map.getTileTraits(tileXLeft, tileYBottom).isSolid;
  bool isSolidLeftTop = _map.getTileTraits(tileXLeft, tileYTop).isSolid;
  bool isSolidRightBottom = _map.getTileTraits(tileXRight, tileYBottom).isSolid;
  bool isSolidRightTop = _map.getTileTraits(tileXRight, tileYTop).isSolid;
  if (isSolidLeftBottom && isSolidLeftTop) {
    correction.x = copysignf(newBox.getOverlap(tileXLeft, tileYBottom, 1, 2).width, -dx);
  }
  else if (isSolidRightBottom && isSolidRightTop) {
    correction.x = copysignf(newBox.getOverlap(tileXRight, tileYBottom, 1, 2).width, -dx);
  }
  
  if (isSolidLeftBottom && isSolidRightBottom) {
    correction.y = copysignf(newBox.getOverlap(tileXLeft, tileYBottom, 2, 1).height, -dy);
  }
  else if (isSolidLeftTop && isSolidRightTop) {
    correction.y = copysignf(newBox.getOverlap(tileXLeft, tileYTop, 2, 1).height, -dy);
  }

  if (correction.x != 0 || correction.y != 0) {
    entity.onTileCollision(correction);
    return;
  }

  struct TileCoord{ UInt x; UInt y; };

  std::array<TileCoord, 4> tilesToCheck{
    TileCoord{tileXLeft, tileYBottom},
    TileCoord{tileXLeft, tileYTop},
    TileCoord{tileXRight, tileYBottom},
    TileCoord{tileXRight, tileYTop},
  };
  
  for (TileCoord& tile : tilesToCheck) {
    Point *v = _getSingleTileCollision(newBox, tile.x, tile.y, dx, dy);
    if (v != nullptr) {
      entity.onTileCollision(*v);
      return;
    };
  }
}

void World::update(float timePassed) {
  if (player != nullptr) {
    detectTileCollision(*player);
    player->update();
  }

  for (auto& enemy : enemies) enemy.update(*player);

  updateCamera();
  updateMissles();
  updateCurrentRoom();
}

void World::drawMap() {
  using std::min;
  using std::max;
  UInt tX0 = max<UInt>(max<UInt>(floor(_cameraX - _halfScreenWidth), 0), ceil(_currentRoom.area.getLeft()));
  UInt tY0 = max<UInt>(max<UInt>(floor(_cameraY - _halfScreenHeight), 0), ceil(_currentRoom.area.getBottom()));
  UInt tXn = min<UInt>(max<UInt>(ceil(_cameraX + _halfScreenWidth), 0), floor(_currentRoom.area.getRight()));
  UInt tYn = min<UInt>(max<UInt>(ceil(_cameraY + _halfScreenHeight), 0), floor(_currentRoom.area.getTop()));
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
  BeginMode2D({ 
    .offset = {_halfScreenWidth * COORD_UNIT, _halfScreenHeight * COORD_UNIT},
    .target = {_cameraX * COORD_UNIT, -_cameraY * COORD_UNIT},
    .rotation = 0,
    .zoom = 1,
  });

    drawMap();
    player->draw();

    drawMissles();

    for (auto& enemy : enemies) enemy.draw();

  EndMode2D();
}
