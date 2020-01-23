#pragma once
#include <string>
#include <list>
#include <vector>
#include "../typeAliases.h"
#include "TileSet.h"

struct EntityDescription {
  IntID id;
  String name;
  String type;
  PixelPoint pixelCoord;
  bool isFacingRight;
};

class Map {
  public:
    std::list<EntityDescription> entities;
    std::vector<TileTypeIndex> tiles;
    TileSet tileSet;
    ULong width = 0;
    ULong height = 0;

    Map() {};
    Map(std::string filename);
};