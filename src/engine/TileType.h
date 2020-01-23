#pragma once
#include <string>
#include "../typeAliases.h"

class TileType {
  public:
    IntID id;
    String type;
    bool isSolid;
    bool isBreakable = false;
    float friction = 1;
    UInt texX;
    UInt texY;

    TileType();
    TileType(
      IntID id,
      UInt texX, UInt texY,
      String type,
      bool isSolid = false,
      bool isBreakable = true
    );
};