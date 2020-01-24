#include "TileSet.h"
#include "../graphics/utils.h"

using namespace TileSetConstants;

TileSet::TileSet() {
  for (UInt i = 0; i < count; i++) {
    addTileTraits(i, "Background");
  }
}

TileSet::TileSet(String filename) {

}

void TileSet::addTileTraits(
  IntID id, String type, bool isSolid, bool isBreakable
) {
  tileTraits.push_back(TileTraits(id, getTileTexX(id), getTileTexY(id), type, isSolid, isBreakable));
}

TileTraits TileSet::operator[](TileTraitsIndex index) {
  return tileTraits[index];
}

void TileSet::drawTile(ULong x, ULong y, TileTraitsIndex index){
  TileTraits &tile = tileTraits[index];
  GLfloat texLeft = (GLfloat)tile.texX / columns + TILE_TEXTURE_SIDES_CUTOFF;
  GLfloat texTop = (GLfloat)tile.texY / rows + TILE_TEXTURE_SIDES_CUTOFF;
  GLfloat texRight = (GLfloat)(tile.texX + 1) / columns - TILE_TEXTURE_SIDES_CUTOFF;
  GLfloat texBottom = (GLfloat)(tile.texY + 1) / rows - TILE_TEXTURE_SIDES_CUTOFF;
  drawSprite(textureId, x, y, x + 1, y + 1, texLeft, texTop, texRight, texBottom);
};
