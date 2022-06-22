#pragma once
#include <string>
#include <vector>
#include <functional>
#include <SDL2/SDL.h>

using Point = SDL_FPoint;

/**
* Track file protocol
* Each track sector must be represented by three parameters (the third could
* be optional theoretically but for now it's mandatory):
*   - the total angle of the curve, 0 means straight road
*   - the length of the curve
*   - the center of the curve, NULL if it's straight one
*
* Each sector must be separated from the next by a newline character
* and the first number must be the total number of sectors
*
* -----------------------------------
*  TOT SECTOR NUMBER
*  ANGLE  LENGTH  CENTER_X,CENTER_Y
*/

typedef struct {
  float angle;
  float length;
  float starting_pendence;
  Point starting_point_or_center;
  std::function<float(float)> f;
} sector;

typedef struct {
  std::vector<sector> sectors;
  float length;
} track;

bool read_track_file(std::string filename, track &t);

unsigned int get_sector_from_distance(float d, track& t);

Point get_world_coord_from_distance(float d, track& t);
