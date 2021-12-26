#pragma once

#include <vector>
#include <string>
#include "TileTraits.h"
#include "../graphics/utils.h"
#include "../typeAliases.h"

class TileSet {
  public:
    String name = "";
    Texture2D texture;
    UInt tileWidth = 16;
    UInt tileHeight = 16;
    UInt count = 256;
    TileTraitsIndex rows = 16;
    TileTraitsIndex columns = 16;
    std::vector<TileTraits> tileTraits;

    TileSet();
    TileSet(String filename);
    
    inline UInt getTileTexX(TileTraitsIndex index) { return index % columns; };
    inline UInt getTileTexY(TileTraitsIndex index) { return index / columns; };
    void addTileTraits(IntID id, String type, bool isSolid = false, bool isBreakable = false);

    TileTraits& operator[](TileTraitsIndex index);
    void drawTile(ULong x, ULong y, TileTraitsIndex index);
};