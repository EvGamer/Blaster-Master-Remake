#pragma once
#include <list>

#include "../../graphics/Animation/Animation.h"
#include "mapArray.h"

struct MissleType {
  Animation *burstAnim;
  Animation *flyAnim;
  float spriteX;
  float spriteY;
  bool foe;
  float damage;
  GLuint *texture;
  bool falling;
};

struct TileType {
  bool solid;
  float friction;
  unsigned char texX;
  unsigned char texY;
};

struct Missle {
//  Animation *burstAnim;
//  Animation *flyAnim;
  float spriteX;
  float spriteY;
  float x;
  float y;
  float speedX;
  float speedY;
  bool hit;
  bool foe;
  bool falling;
  float damage;
//  ~Missle() {
//    delete burstAnim;
//    delete flyAnim;
//  }
};

class World {
 private:
  unsigned char _num;  //���������� ������
  float _size;
  unsigned _lineLenght;  //������ � ������.
  float _friction;
  float _gravity;
  TileType _tile[256];

  GLuint _texture;
  static const unsigned _MAP_SX = MAP_LENGTH;
  static const unsigned _MAP_SY = MAP_HEIGHT;
  // Map, hardcoded in haste
  unsigned char m_map[_MAP_SX * _MAP_SY];

  std::list<Missle> *_missles;

 public:
  void addMissle(float x, float y, float speedX, float speedY, MissleType *wpn);
  float hit(float x1, float y1, float x2, float y2, bool foe);
  bool collide(float x, float y);
  inline float getGravity() {
    return _gravity;
  };
  inline void setGravity(float a_gravity) {
    _gravity = a_gravity;
  };
  bool getFrict(unsigned char i, unsigned char j);
  void update();
  void draw(int cx, int cy, unsigned char a_num);
  void set(unsigned char i, int tx, int ty, bool solid, float fric);
  void set(unsigned char i, int tx, int ty, bool solid);
  inline void setGlobalFriction(float a_friction) {
    _friction = a_friction;
  };
  void setDefault(unsigned first, unsigned last);
  void setSolid(unsigned first, unsigned last);
  void add(int tx, int ty, bool solid, float fric);
  void add(int tx, int ty, bool solid);
  void drawLevel(float scrX, float scrY);
  World(std::string fileName, float TilesInLine);
};
