#pragma once

const unsigned COORD_UNIT = 32;
const unsigned WINDOW_WIDTH = 1024;
const unsigned WINDOW_HEIGHT = 640;
const int FACING_LEFT = -1;
const int FACING_RIGHT = 1;
constexpr unsigned TILE_COLUMNS = WINDOW_WIDTH / COORD_UNIT;
constexpr unsigned TILE_ROWS = WINDOW_HEIGHT / COORD_UNIT;