#include "TileSet.h"
#include "../graphics/utils.h"
#include "../typeAliases.h"
#include "../thirdParty/tinyxml2/tinyxml2.h"

TileSet::TileSet() {
  for (UInt i = 0; i < count; i++) {
    addTileTraits(i, "Background");
  }
}

TileSet::TileSet(String filename) {
  typedef tinyxml2::XMLElement Tag;

  tinyxml2::XMLDocument doc;
  doc.LoadFile(filename.c_str());

  Tag* tileSetTag = doc.FirstChildElement("tileset");
  tileHeight = tileSetTag->UnsignedAttribute("tileheight");
  tileWidth = tileSetTag->UnsignedAttribute("tilewidth");
  count = tileSetTag->UnsignedAttribute("tilecount");
  columns = tileSetTag->UnsignedAttribute("columns");
  rows = count / columns;
  tileTraits = std::vector<TileTraits>(count);

  Tag* imageTag = tileSetTag->FirstChildElement("image");
  texture = TextureResource(imageTag->Attribute("source"));

  Tag* tileTag = tileSetTag->FirstChildElement("tile");
  
  while (tileTag != nullptr) {
    bool isBreakable = false;
    bool isSolid = false;

    UInt id = tileTag->UnsignedAttribute("id");
    UInt index = id + 1;
    tileTraits[index].texX = getTileTexX(id);
    tileTraits[index].texY = getTileTexY(id);

    Tag* propTag = tileTag->FirstChildElement("properties");
    if (propTag != nullptr) propTag = propTag->FirstChildElement("property");
    
    while (propTag != nullptr) {
      if (String(propTag->Attribute("name")) == "isBreakable") {
        tileTraits[index].isBreakable = propTag->BoolAttribute("value");
      }
      else if (String(propTag->Attribute("name")) == "isSolid") {
        tileTraits[index].isSolid = propTag->BoolAttribute("value");
      }

      propTag = propTag->NextSiblingElement("property");
    }

    auto typeName = tileTag->Attribute("type");
    if (typeName != nullptr) tileTraits[id].type = typeName;

    tileTag = tileTag->NextSiblingElement("tile");
  }
}

void TileSet::addTileTraits(
  IntID id, String type, bool isSolid, bool isBreakable
) {
  tileTraits.push_back(TileTraits(id, getTileTexX(id), getTileTexY(id), type, isSolid, isBreakable));
}

TileTraits& TileSet::operator[](TileTraitsIndex index) {
  return tileTraits[index];
}

void TileSet::drawTile(ULong x, ULong y, TileTraitsIndex index){
  TileTraits &tile = tileTraits[index];
  float texLeft = (float)tile.texX / columns;
  float texTop = (float)tile.texY / rows;
  float texRight = (float)(tile.texX + 1) / columns;
  float texBottom = (float)(tile.texY + 1) / rows;
  drawSprite(texture, x, y, x + 1, y + 1, texLeft, texTop, texRight, texBottom);
};
