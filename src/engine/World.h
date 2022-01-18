#pragma once
#include <list>
#include <array>
#include <memory>

#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../graphics/TextureKeeper.h"
#include "../utils/Point.h"
#include "../typeAliases.h"
#include "IWorld.h"
#include "constants.h"
#include "TileTraits.h"
#include "Map.h"
#include "Room.h"


struct WorldTextureFileNames {
  String player;
  String playerMissle;
  String enemy;
  String area;
};

class World: public IWorld{

 public:
  std::list<Enemy> enemies;
  std::unique_ptr<Player> player;

  float _cameraX = 0;
  float _cameraY = 0;

  void init();
  void loadTextures(WorldTextureFileNames fileNams);
  void addMissle(float x, float y, float speedX, float speedY, MissleTraits *wpn) override final;
  float hit(float x1, float y1, float x2, float y2, bool foe) override final;
  bool isSolidTileAtCoord(float x, float y) override;
  float getGravity() override {
    return _gravity;
  };
  inline void setGravity(float a_gravity) {
    _gravity = a_gravity;
  };
  bool getTileFriction(unsigned char i, unsigned char j) override final;
  void update(float timePassed);
  inline void setGlobalFriction(float a_friction) {
    _friction = a_friction;
  };
  bool isPlayerInRoom(Room& room);
  void detectTileCollision(Entity& entity);
  void updateCamera();
  void updateCurrentRoom();
  void updateMissles();
  void drawMap();
  void drawMissles();
  void draw();
  World();
 private:
  float _friction;
  float _gravity;
  float _halfScreenWidth = TILE_COLUMNS / 2;
  float _halfScreenHeight = TILE_ROWS / 2;
  Room _currentRoom;  
  //ToDo make singletones in classes what use them
  TextureKeeper _playerTexture;
  TextureKeeper _playerMissleTexture;
  TextureKeeper _enemyTexture;
  Map _map;

  TextureKeeper _texture;
  // Map, hardcoded in haste

  std::list<Missle> _missles;

  WorldVector _getSingleTileCollision(Rect &entity, UInt tileX, UInt tileY, float dx, float dy);
};
