#pragma once

#include <vector>
#include <string>
#include "TileType.h"
#include "../graphics/utils.h"
#include "../typeAliases.h"

class TileSet {
  public:
    String name = "";
    GLuint textureId = 0;
    UInt tileWidth = 16;
    UInt tileHeight = 16;
    TileTypeIndex count = 256;
    TileTypeIndex rows = 16;
    TileTypeIndex columns = 16;
    std::vector<TileType> tileTypes;

    TileSet() {};
    TileSet(String filename);
    void addTileType(IntID id, String type, bool isSolid = true, bool isBreakable = false);
};