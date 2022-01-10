#pragma once
#include "../typeAliases.h"
#include "../engine/constants.h"

template <typename T>
struct Vector2D {
  T x;
  T y;
  
  constexpr Vector2D<T> operator* (const T multiplier) {
    return { x * multiplier, y * multiplier };
  };
  
  constexpr Vector2D<float> operator/ (const float divisor) {
    return { x / divisor, y / divisor };
  }

  constexpr Vector2D<T> operator+ (const Vector2D<T>& operand) {
    return { x + operand.x, y + operand.y };
  }

  constexpr Vector2D<T> operator- (const Vector2D<T>& operand) {
    return { x - operand.x, y - operand.x };
  }
};

typedef Vector2D<UInt> TextureVector;
typedef Vector2D<Long> ScreenVector;
typedef Vector2D<float> WorldVector;

constexpr WorldVector toWorldVector(ScreenVector screenVector) {
  return WorldVector({
    (float)screenVector.x / COORD_UNIT,
    -(float)screenVector.y / COORD_UNIT
  });
}