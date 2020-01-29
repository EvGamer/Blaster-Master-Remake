#pragma once
#include "../typeAliases.h"
#include "../utils/Rectangle.h"

class Room {
  public:
    IntID id = 0;
    String name;
    String type;
    Rect area;
    Room(IntID id, Rect area, const char* name = "Room 0", const char* type = "Room");
    Room(IntID id, Rect area, String name, String type);
};