
#include "World.h"
#include "Map.h"
#include "../typeAliases.h"

World::World(String textureFileName, float TilesInLine) {
  // don't ask.
  for (int i = 0; i <= _MAP_SY; i++) {
    for (int j = 0; j < _MAP_SY; j++) {
      int c = (_MAP_SY - 1 - j) * _MAP_SX + i;
      m_map[c] = MAP_ARRAY[c];  //
    };
  };

  Map map("maps/Test.tmx");

  _num = 0;
  _size = 1 / TilesInLine;
  _lineLenght = TilesInLine;
  _gravity = 1;
  _friction = 0;
  _texture = loadTexture(textureFileName);
  _missles = new std::list<Missle>();
}

World::~World() {
  delete _missles;
}

void World::addMissle(float x, float y, float speedX, float speedY,
                    MissleType *wpn) {
  _missles->push_front({
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

  for (auto &missle : *_missles) {
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
  unsigned i = floor(x);
  unsigned j = floor(y);
  return _tileSet[m_map[(_MAP_SY - 1 - j) * _MAP_SX + i] - 1].isSolid;
};

bool World::getFrict(unsigned char i, unsigned char j) {
  return _tileSet[m_map[(_MAP_SY - 1 - j) * _MAP_SX + i] - 1].friction;
}

bool isMissleExploded(Missle &value) {
  return value.burstAnim.isEnded();
}

void World::update() {
  for (auto &missle : *_missles) {
    if (!missle.hit) {
      missle.x += missle.speedX;
      missle.y += missle.speedY;
      if (missle.falling) missle.speedY -= 0.1 * _gravity;
      missle.hit = collide(missle.x, missle.y);
    }
  }
  _missles->remove_if(isMissleExploded);
}

void World::draw(int cx, int cy, unsigned char a_num) {
  float tx = _tileSet[a_num].texX * _size;
  float ty = _tileSet[a_num].texY * _size;
  float b = 0.001; // ToDo improve tile rendering and remove this crutch
  // cy=m_lineLenght-1-cy;
  drawSprite(_texture, cx, cy, cx + 1, cy + 1, tx + b, ty + b,
             tx + _size - b, ty + _size - b);
};

void World::set(unsigned char i, int tx, int ty, bool isSolid, float fric) {
  _tileSet[i].isSolid = isSolid;
  _tileSet[i].texX = tx;
  _tileSet[i].texY = _lineLenght - ty;
  _tileSet[i].friction = fric;
}

void World::set(unsigned char i, int tx, int ty, bool isSolid) {
  _tileSet[i].isSolid = isSolid;
  _tileSet[i].texX = tx;
  _tileSet[i].texY = ty;
  if (isSolid) {
    _tileSet[i].friction = _friction;
  } else {
    _tileSet[i].friction = 0;
  };

  /// float tileFriction = 0;
  // if (isSolid) tileFriction = friction;
  // set(i,tx,ty,isSolid,tileFriction);
}

void World::setDefault(unsigned first, unsigned last) {
  for (unsigned i = first - 1; i < last; i++)
    set(i, (i) % _lineLenght, (i) / _lineLenght, false);
}

void World::setSolid(unsigned first, unsigned last) {
  for (unsigned i = first; i <= last; i++) _tileSet[i - 1].isSolid = true;
}

void World::add(int tx, int ty, bool isSolid, float fric) {
  set(_num, tx, ty, isSolid, fric);
  _num++;
}

void World::add(int tx, int ty, bool isSolid) {
  set(_num, tx, ty, isSolid);
  _num++;
}

void World::drawLevel(float scrX, float scrY) {
  unsigned tX0 = floor(scrX);
  unsigned tY0 = floor(scrY);
  unsigned tXn = ceil(scrX + 32);
  unsigned tYn = ceil(scrY + 24);
  for (int i = tX0; i <= tXn; i++) {
    for (int j = tY0; j < tYn; j++) {
      draw(i, j, m_map[(_MAP_SY - 1 - j) * _MAP_SX + i] - 1);  //
    };
  };
  char dir;
  float x0;
  float x1, x2;
  float y0;
  for (auto &missle : *_missles) {
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
