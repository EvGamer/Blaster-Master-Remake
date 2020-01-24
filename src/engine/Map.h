#pragma once
#include <string>
#include <list>
#include <vector>
#include "TileSet.h"
#include "../typeAliases.h"

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
    std::vector<TileTraitsIndex> tiles;
    TileSet tileSet;
    ULong width = 0;
    ULong height = 0;

    Map() {};
    Map(String filename);

    inline bool coordOutOfRange(long x, long y) {
      return x < 0 || y < 0 || x >= width || y >= height;
    }
    inline TileTraitsIndex getTileTraitsIndex(ULong x, ULong y) {
      return tiles[((ULLong)(height - 1l - y) * width) + x] - 1;
    };
    TileTraits getTileTraits(ULong x, ULong y);
    void drawTile(ULong x, ULong y);
};