#include "Map.h"
#include "../thirdParty/tinyxml2/tinyxml2.h"

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

Map::Map(String filename) {
  typedef tinyxml2::XMLElement Tag;

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
  
  Tag* objTag = objGroupTag->FirstChildElement("object");
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

    long entityY = height * tileSet.tileWidth - objTag->IntAttribute("y");

    entities.push_back({
      objTag->UnsignedAttribute("id"),
      String(objTag->Attribute("name")),
      String(objTag->Attribute("type")),
      {
        (long)objTag->IntAttribute("x"),
        entityY
      },
      isFacingRight,
    });

    objTag = objTag->NextSiblingElement("object");
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
