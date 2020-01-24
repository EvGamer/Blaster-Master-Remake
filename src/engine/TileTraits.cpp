#include "TileTraits.h"

TileTraits::TileTraits() {
  id = 0;
  texX = 0;
  texY = 0;
  type = "Background";
  isSolid = false;
}

TileTraits::TileTraits(
  IntID a_id,
  UInt a_texX, UInt a_texY,
  String a_type,
  bool a_isSolid,
  bool a_isBreakable
) {
  id = a_id;
  texX = a_texX;
  texY = a_texY;
  type = a_type;
  isSolid = a_isSolid;
  isBreakable = a_isBreakable;
}