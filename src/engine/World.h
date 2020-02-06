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
#include "Room.h"


class World: public IWorld{
 private:
  float _friction;
  float _gravity;
  float _halfScreenWidth = 16;
  float _halfScreenHeight = 12;
  float _cameraX = 0;
  float _cameraY = 0;
  Room _currentRoom;  
  //ToDo make singletones in classes what use them
  GLuint _playerTextureId;
  GLuint _playerMissleTextureId;
  GLuint _enemyTextureId;
  Map _map;

  GLuint _texture;
  // Map, hardcoded in haste

  std::list<Missle> _missles;

  Point* _getSingleTileCollision(Rect &entity, UInt tileX, UInt tileY, float dx, float dy);

 public:
  std::list<Enemy> enemies;
  Player* player;

  void init();
  void addMissle(float x, float y, float speedX, float speedY, MissleTraits *wpn);
  float hit(float x1, float y1, float x2, float y2, bool foe);
  bool isSolidTileAtCoord(float x, float y);
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
  bool isPlayerInRoom(Room& room);
  void applyCamera();
  void detectTileCollision(Entity& entity);
  void updateCamera();
  void updateCurrentRoom();
  void updateMissles();
  void drawMap();
  void drawMissles();
  void draw();
  World(String fileName);
  ~World();
};
