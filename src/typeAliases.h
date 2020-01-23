#pragma once

typedef unsigned int UInt;
typedef unsigned char Byte;
typedef unsigned long ULong;
typedef std::string String;
typedef Byte TileTypeIndex;
typedef ULong IntID;

template <typename T>
struct Point2D {
  T x;
  T y;
};

typedef Point2D<UInt> TexPoint;
typedef Point2D<long> PixelPoint;
typedef Point2D<float> Point;