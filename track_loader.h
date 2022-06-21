#pragma once
#include <fstream>
#include <vector>

/**
* Track file protocol
* Each track sector must be represented by three parameters (the third could
* be optional theoretically but for now it's mandatory):
*   - the total angle of the curve, 0 means straight road
*   - the length of the curve
*   - the center of the curve, NULL if it's straight one
*
* Each sector must be separated from the next by a space character
* and the first number must contain the total number of sectors
*
* -----------------------------------
*  TOT SECTOR NUMBER
*  ANGLE  LENGTH  CENTER_X,CENTER_Y
*/

typedef struct {
  float angle;
  float length;
  std::pair<float, float> center;
} sector;

bool read_track_file(std::string filename, std::vector<sector>& track);
