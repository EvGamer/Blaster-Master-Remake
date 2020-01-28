#pragma once
#include <list>
#include <array>
#include <memory>

#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../typeAliases.h"
#include "IWorld.h"
#include "TileTraits.h"
#include "Map.h"





class World: public IWorld{
 private:
  float _friction;
  float _gravity;
  
  //ToDo make singletones in classes what use them
  GLuint _playerTextureId;
  GLuint _playerMissleTextureId;
  GLuint _enemyTextureId;
  Map _map;

  GLuint _texture;
  // Map, hardcoded in haste

  std::list<Missle> _missles;


 public:
  std::list<Enemy> enemies;
  Player* player;

  void init();
  void addMissle(float x, float y, float speedX, float speedY, MissleTraits *wpn);
  float hit(float x1, float y1, float x2, float y2, bool foe);
  bool collide(float x, float y);
  float getGravity() override {
    return _gravity;
  };
  inline void setGravity(float a_gravity) {
    _gravity = a_gravity;
  };
  bool getFrict(unsigned char i, unsigned char j);
  void update();
  inline void setGlobalFriction(float a_friction) {
    _friction = a_friction;
  };
  void setSolid(unsigned first, unsigned last);
  void drawLevel(float scrX, float scrY);
  World(String fileName);
  ~World();
};
