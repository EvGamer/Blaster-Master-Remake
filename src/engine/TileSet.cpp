#include "TileSet.h"

TileSet::TileSet(String filename) {

}

void TileSet::addTileType(
  IntID id, String type, bool isSolid, bool isBreakable
)
{
  TileTypeIndex texY = id / columns;
  TileTypeIndex texX = id % columns;
  tileTypes.push_back(TileType(id, texX, texY, type, isSolid, isBreakable));
};
