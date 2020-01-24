#include "Map.h"
#include "../thirdParty/tinyxml2/tinyxml2.h"

Map::Map(String filename) {

}

TileTraits Map::getTileTraits(ULong x, ULong y) {
  return tileSet[getTileTraitsIndex(x, y)];
}

void Map::drawTile(ULong x, ULong y){
  tileSet.drawTile(x, y, getTileTraitsIndex(x, y));
};
