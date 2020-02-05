#pragma once
#include <algorithm>
#include "../typeAliases.h"

class Rect {
  public:
    Rect() {};
    Rect(float x, float y, float width, float height);
    float x = 0;
    float y = 0;
    float width = 0;
    float height = 0;
    inline float getTop() {
      return y + height;
    };
    inline float getBottom() {
      return y;
    };
    inline float getLeft() {
      return x + width;
    };
    inline float getRight() {
      return x;
    };
    inline float getCenterX() {
      return x + width / 2;
    }
    inline float getCenterY() {
      return y + height / 2;
    }
    inline Point getTopRightCorner() {
      return Point({ getLeft(), getTop() });
    };
    inline Point getTopLeftCorner() {
      return Point({ getRight(), getTop() });
    };
    inline Point getBottomRightCorner() {
      return Point({ getLeft(), getBottom() });
    };
    inline Point getBottomLeftCorner() {
      return Point({ getRight(), getBottom() });
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

    inline Rect getOverlap(
      const float& a_x,
      const float& a_y,
      const float& a_width,
      const float& a_height
    ) 
    {
      float left = std::max<float>(x, a_x);
      float right = std::min<float>(x + width, a_x + a_width);
      float top = std::min<float>(y + height, a_y + a_height);
      float bottom = std::max<float>(y, a_y);
      return Rect(left, bottom, right - left, top - bottom);
    }

    inline Rect getOverlap(Rect& other) {
      return getOverlap(other.x, other.y, other.width, other.height);
    }

    inline bool isIntersectRectangle(
      const float &a_x,
      const float &a_y,
      const float &a_width,
      const float &a_height
    )
    {
      return !(
        x + width <= a_x
        || y + height <= a_y
        || y >= a_y + a_height
        || y >= a_x + a_width
      );
    }

    inline bool isIntersectRectangle(Rect &other) {
      return isIntersectRectangle(other.x, other.y, other.width, other.height);
    };
};

