#include "Map.h"
#include "../thirdParty/tinyxml2/tinyxml2.h"

Map::Map() {
  entities = std::list<EntityDescription>({
    { 2, "Enemy 1", "Enemy", {tileToPixelX(22), tileToPixelY(11)}, false },
    { 3, "Enemy 2", "Enemy", {tileToPixelX(22), tileToPixelY(11)}, false },
    { 4, "Enemy 3", "Enemy", {tileToPixelX(24), tileToPixelY(16)}, true },
    { 5, "Enemy 4", "Enemy", {tileToPixelX(30), tileToPixelY(16)}, true },
    { 6, "Enemy 5", "Enemy", {tileToPixelX(12), tileToPixelY(24)}, false },
    { 7, "Enemy 6", "Enemy", {tileToPixelX(18), tileToPixelY(20)}, false },
    { 8, "Enemy 7", "Enemy", {tileToPixelX(19), tileToPixelY(28)}, true },
    { 9, "Enemy 8", "Enemy", {tileToPixelX(27), tileToPixelY(32)}, true },
    { 10, "Enemy 9", "Enemy", {tileToPixelX(35), tileToPixelY(36)}, true },
    { 11, "Enemy 10", "Enemy", {tileToPixelX(43), tileToPixelY(43)}, false },
    { 12, "Enemy 11", "Enemy", {tileToPixelX(22), tileToPixelY(44)}, false },
    { 13, "Enemy 12", "Enemy", {tileToPixelX(9), tileToPixelY(49)}, true },
    { 14, "Enemy 13", "Enemy", {tileToPixelX(15), tileToPixelY(53)}, false },
    { 15, "Enemy 14", "Enemy", {tileToPixelX(29), tileToPixelY(49)}, false },
    { 16, "Enemy 15", "Enemy", {tileToPixelX(36), tileToPixelY(53)}, false },
    { 17, "Enemy 16", "Enemy", {tileToPixelX(46), tileToPixelY(49)}, false },
    { 18, "Enemy 17", "Enemy", {tileToPixelX(52), tileToPixelY(40)}, true },
    { 19, "Enemy 18", "Enemy", {tileToPixelX(52), tileToPixelY(32)}, false },
    { 20, "Enemy 19", "Enemy", {tileToPixelX(52), tileToPixelY(24)}, false },
    { 21, "Enemy 20", "Enemy", {tileToPixelX(52), tileToPixelY(16)}, false },
    { 22, "Enemy 21", "Enemy", {tileToPixelX(61), tileToPixelY(36)}, false },
    { 23, "Enemy 22", "Enemy", {tileToPixelX(61), tileToPixelY(28)}, false },
    { 24, "Enemy 23", "Enemy", {tileToPixelX(61), tileToPixelY(20)}, false },
  });
};

Map::Map(String filename) {

}

TileTraits Map::getTileTraits(ULong x, ULong y) {
  return tileSet[getTileTraitsIndex(x, y)];
}

void Map::drawTile(ULong x, ULong y){
  tileSet.drawTile(x, y, getTileTraitsIndex(x, y));
};