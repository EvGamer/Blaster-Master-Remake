#pragma once
#include <algorithm>
#include "../typeAliases.h"
#include "../utils/Point.h"

class Rect {
  public:
    Rect() {};
    Rect(float x, float y, float width, float height);
    float x = 0;
    float y = 0;
    float width = 0;
    float height = 0;
    inline float top() {
      return y + height;
    };
    inline float bottom() {
      return y;
    };
    inline float right() {
      return x + width;
    };
    inline float left() {
      return x;
    };
    inline float centerX() {
      return x + width / 2;
    }
    inline float centerY() {
      return y + height / 2;
    }
    inline WorldVector topRight() {
      return WorldVector({ right(), top() });
    };
    inline WorldVector topLeft() {
      return WorldVector({ left(), top() });
    };
    inline WorldVector bottomRight() {
      return WorldVector({ right(), bottom() });
    };
    inline WorldVector bottomLeft() {
      return WorldVector({ left(), bottom() });
    };
    inline bool isContainPoint(float a_x, float a_y) {
      return (
        x < a_x && a_x < x + width
        && y < a_y && a_y < y + height
      );
    };
    inline bool isContainPoint(WorldVector &point) {
      return isContainPoint(point.x, point.y);
    };
    inline bool isContainRectangle(float a_x, float a_y, float a_width, float a_height) {
      return (
        x < a_x && a_x + a_width < x + width
        && y < a_y && a_y + a_height < y + height
      );
    };
    inline bool isContainRectangle(const Rect &otherRect) {
      return isContainRectangle(
        otherRect.x,
        otherRect.y,
        otherRect.width,
        otherRect.height
      );
    };

    inline Rect getOverlap(
      float a_x,
      float a_y,
      float a_width,
      float a_height
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
      float a_x,
      float a_y,
      float a_width,
      float a_height
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

