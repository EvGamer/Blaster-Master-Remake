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
  private:
    void _parseTilesFromCsv(const char* tilesCsv);
  public:
    std::list<EntityDescription> entities;
    std::vector<TileTraitsIndex> tiles;
    TileSet tileSet;
    ULong width = 0;
    ULong height = 0;

    Map();
    Map(String filename);

    inline bool coordOutOfRange(long x, long y) {
      return x < 0 || y < 0 || x >= width || y >= height;
    }
    inline TileTraitsIndex getTileTraitsIndex(ULong x, ULong y) {
      return tiles[(ULLong)((height - y - 1) * width) + x];
    };
    inline long tileToPixelX(float x) { return round((double)x * tileSet.tileWidth); }
    inline long tileToPixelY(float y) { return round((double)y * tileSet.tileHeight); }
    inline Point pixelToTileCoord(PixelPoint p) {
      return {
        (float)p.x / tileSet.tileWidth,
        (float)p.y / tileSet.tileHeight,
      };
    }
    inline float pixelToTileY(long y) { return y / tileSet.tileHeight; }
    TileTraits getTileTraits(ULong x, ULong y);
    void drawTile(ULong x, ULong y);
};