
#include <vector>
#include "World.h"

World::World(String textureFileName) {
  _map.tiles = std::vector<TileTraitsIndex>(MAP_ARRAY.begin(), MAP_ARRAY.end());
  _map.width = MAP_LENGTH;
  _map.height = MAP_HEIGHT;
  _map.tileSet.textureId = loadTexture(textureFileName);

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

bool World::collide(float x, float y) {
  if (_map.coordOutOfRange(x, y)) return false;
  return !_map.coordOutOfRange(x, y) && _map.getTileTraits(floor(x), floor(y)).isSolid;
};

bool World::getFrict(unsigned char i, unsigned char j) {
  return !_map.coordOutOfRange(i, j) && _map.getTileTraits(i, j).isSolid ? _friction : 0;
}

bool isMissleExploded(Missle &value) {
  return value.burstAnim.isEnded();
}

void World::update() {
  for (auto &missle : _missles) {
    if (!missle.hit) {
      missle.x += missle.speedX;
      missle.y += missle.speedY;
      if (missle.falling) missle.speedY -= 0.1 * _gravity;
      missle.hit = collide(missle.x, missle.y);
    }
  }
  _missles.remove_if(isMissleExploded);
}

void World::setSolid(unsigned first, unsigned last) {
  for (unsigned i = first; i <= last; i++) _map.tileSet[i - 1].isSolid = true;
}

void World::drawLevel(float scrX, float scrY) {
  UInt tX0 = floor(scrX);
  unsigned tY0 = floor(scrY);
  unsigned tXn = ceil(scrX + 32);
  unsigned tYn = ceil(scrY + 24);
  for (int i = tX0; i <= tXn; i++) {
    for (int j = tY0; j < tYn; j++) {
      _map.drawTile(i, j);
    };
  };
  char dir;
  float x0;
  float x1, x2;
  float y0;
  for (auto &missle : _missles) {
    if (missle.speedX < 0) {
      dir = -1;
      x1 = missle.x - missle.spriteX;
      x0 = x1 - 2;
      x2 = x0;
    } else {
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
