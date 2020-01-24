#pragma once
#include <string>
#include "../typeAliases.h"

class TileTraits {
  public:
    IntID id;
    String type;
    bool isSolid;
    bool isBreakable = false;
    float friction = 1;
    UInt texX;
    UInt texY;

    TileTraits();
    TileTraits(
      IntID id,
      UInt texX, UInt texY,
      String type,
      bool isSolid = false,
      bool isBreakable = true
    );
};