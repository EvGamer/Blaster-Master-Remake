#pragma once
#include "../typeAliases.h"

class Rect {
  public:
    Rect() {};
    Rect(float x, float y, float width, float height);
    float x = 0;
    float y = 0;
    float width = 0;
    float height = 0;
    inline float getNorth() {
      return y + height;
    };
    inline float getSouth() {
      return y;
    };
    inline float getEast() {
      return x + width;
    };
    inline float getWest() {
      return x;
    };
    inline float getCenterX() {
      return x + width / 2;
    }
    inline float getCenterY() {
      return y + height / 2;
    }
    inline Point getTopRightCorner() {
      return Point({ getEast(), getNorth() });
    };
    inline Point getTopLeftCorner() {
      return Point({ getWest(), getNorth() });
    };
    inline Point getBottomRightCorner() {
      return Point({ getEast(), getSouth() });
    };
    inline Point getBottomLeftCorner() {
      return Point({ getWest(), getSouth() });
    };
    inline bool isContainPoint(float a_x, float a_y) {
      return (
        x < a_x && a_x < x + width
        && y < a_y && a_y < y + height
      );
    };
    inline bool isContainPoint(Point &point) {
      return isContainPoint(point.x, point.y);
    };
    inline bool isContainRectangle(float a_x, float a_y, float a_width, float a_height) {
      return (
        x < a_x && a_x + a_width < x + width
        && y < a_y && a_y + a_height < y + height
      );
    };
    inline bool isContainRectangle(Rect &otherRect) {
      return isContainRectangle(
        otherRect.x,
        otherRect.y,
        otherRect.width,
        otherRect.height
      );
    };
    inline bool isIntersectRectangle(Rect &otherRect) {
      return (
        isContainPoint(otherRect.getTopRightCorner())
        || isContainPoint(otherRect.getTopLeftCorner())
        || isContainPoint(otherRect.getBottomRightCorner())
        || isContainPoint(otherRect.getBottomLeftCorner())
        || (
          getEast() < otherRect.getEast() && otherRect.getWest() < getWest()
          && otherRect.getSouth() < getSouth() && getNorth() < otherRect.getNorth()
        )
        || (
          getEast() > otherRect.getEast() && otherRect.getWest() > getWest()
          && otherRect.getSouth() > getSouth() && getNorth() > otherRect.getNorth()
        )
      );
    };
};

