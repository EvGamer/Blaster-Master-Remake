#include "Map.h"
#include "../utils/Rectangle.h"

Map::Map() {
};

void Map::_parseTilesFromCsv(const char* tilesCsv) {
  char* tileCsv = strdup(tilesCsv);
  char* rawTileIndex = strtok(tileCsv, ",");

  while (rawTileIndex) {
    unsigned tileIndex;
    std::sscanf(rawTileIndex, "%u", &tileIndex);
    tiles.push_back(tileIndex);
    rawTileIndex = strtok(NULL, ",");
  }
}

void Map::_parseEntityGroup(Tag &objGroupTag) {
  Tag* objTag = objGroupTag.FirstChildElement("object");
  while (objTag != nullptr) {

    bool isFacingRight = false;

    Tag* propTag = objTag->FirstChildElement("properties");
    if (propTag != nullptr) propTag = propTag->FirstChildElement("property");
    while (propTag != nullptr) {
      if (String(propTag->Attribute("name")) == "isFacingRight") {
        isFacingRight = propTag->BoolAttribute("value");
      }
      propTag = propTag->NextSiblingElement("property");
    }

    entities.push_back({
      objTag->UnsignedAttribute("id"),
      String(objTag->Attribute("name")),
      String(objTag->Attribute("type")),
      {
        (long)objTag->IntAttribute("x"),
        (long)objTag->IntAttribute("y"),
      },
      isFacingRight,
    });

    objTag = objTag->NextSiblingElement("object");
  }
};

void Map::_parseRoomGroup(Tag &objGroupTag) {
  Tag* objTag = objGroupTag.FirstChildElement("object");
  while (objTag != nullptr) {

    bool isFacingRight = false;

    float roomHeight = objTag->UnsignedAttribute("height") / tileSet.tileHeight;
    float roomWidth = objTag->UnsignedAttribute("width") / tileSet.tileWidth;
    float roomX = pixelToTileX(objTag->IntAttribute("x"));
    float roomY = pixelToTileY(objTag->IntAttribute("y")) - roomHeight;

    rooms.push_back(Room({
      objTag->UnsignedAttribute("id"),
      Rect(roomX, roomY, roomWidth, roomHeight),
      objTag->Attribute("name"),
      objTag->Attribute("type"),
    }));

    objTag = objTag->NextSiblingElement("object");
  }
};

Map::Map(String filename) {
  tinyxml2::XMLDocument doc;
  doc.LoadFile(filename.c_str());

  Tag* mapTag = doc.FirstChildElement("map");
  if (mapTag == nullptr) throw "Failed to load map. Couldn't find <map> tag";
  
  width = mapTag->UnsignedAttribute("width");
  height = mapTag->UnsignedAttribute("height");

  Tag* layerTag = mapTag->FirstChildElement("layer");

  Tag* dataTag = layerTag->FirstChildElement("data");
  String encoding = String(dataTag->Attribute("encoding"));
  if (encoding != "csv") {
    throw "Map data encoding type is not csv";
  };

  _parseTilesFromCsv(dataTag->GetText());

  Tag* tileSetTag = mapTag->FirstChildElement("tileset");
  tileSet = TileSet(String(tileSetTag->Attribute("source")));

  Tag* objGroupTag = mapTag->FirstChildElement("objectgroup");
  
  while (objGroupTag != nullptr) {
    String groupName(objGroupTag->Attribute("name"));
    if (groupName == "Entities") {
      _parseEntityGroup(*objGroupTag);
    }
    else if (groupName == "Rooms") {
      _parseRoomGroup(*objGroupTag);
    }

    objGroupTag = objGroupTag->NextSiblingElement("objectgroup");
  }

}

TileTraits Map::getTileTraits(ULong x, ULong y) {
  return tileSet[getTileTraitsIndex(x, y)];
}

void Map::drawTile(ULong x, ULong y){
  if (x >= width || y >= height) return;
  TileTraitsIndex index = getTileTraitsIndex(x, y);
  if (index == 0) return;
  tileSet.drawTile(x, y, index);
};
