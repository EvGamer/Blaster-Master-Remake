#include "Room.h"

Room::Room(IntID a_id, Rect a_area, String a_name, String a_type) {
  id = a_id;
  area = a_area;
  name = a_name;
  type = a_type;
}

Room::Room(IntID a_id, Rect a_area, const char* a_name, const char* a_type) {
  id = a_id;
  area = a_area;
  name = String(a_name);
  type = String(a_type);
}