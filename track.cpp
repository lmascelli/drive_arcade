#include "track.h"
#include "utility.h"
#include <cassert>
#include <fstream>
#include <sstream>

bool read_track_file(std::string filename, track &t) {
  using namespace std;
  ifstream in(filename);
  if (!in.is_open())
    return false;
  try {
    char buffer[64];
    unsigned int number_of_sectors;
    in.getline(buffer, 64);
    stringstream ss(buffer);

    ss >> number_of_sectors;
    printf("reading %d sectors\n\n", number_of_sectors);

    float sector_starting_pendence = 0;
    Point sector_starting_point = {0.f, 0.f};

    for (unsigned int i = 0; i < number_of_sectors; i++) {
      in.getline(buffer, 64);
      stringstream ss(buffer);
      float angle, length;
      ss >> angle >> length;
      angle *= (M_PI * 2 / 360.f);

      printf("sector %d\n"
             "angle:  %f\n"
             "length: %f\n\n\n",
             i, angle, length);

      std::function<float(float)> f;
      Point sector_starting_point_or_center;

      if (angle != 0.f) {
        float radius = length / angle;
        Point center = track_to_world({(angle > 0.f ? 1 : -1) * radius, 0.f},
                                      sector_starting_pendence);
        f = [radius, center](float x) {
          return sqrt(radius * radius - (x - center.x) * (x - center.x)) +
                 center.y;
        };
        sector_starting_point_or_center = center;
      } else {
        f = [sector_starting_pendence, sector_starting_point](float x) {
          return tan(sector_starting_pendence) * (x - sector_starting_point.x) +
                 sector_starting_point.y;
        };
        sector_starting_point_or_center = sector_starting_point;
      }
      t.sectors.push_back({angle, length, sector_starting_pendence,
                           sector_starting_point_or_center, f});
      t.length += length;
      printf("track loaded. total length = %f\n", t.length);
    }
  } catch (...) {
    printf("ERROR LOADING TRACK\n");
    return false;
  }
  in.close();
  return true;
}

unsigned int get_sector_from_distance(float d, track &t) {
  float cumulative_distance = 0.f;
  unsigned int ret = 0;
  while (cumulative_distance <= d) {
    cumulative_distance += t.sectors[ret].length;
    assert(cumulative_distance <= t.length);
    ret++;
  }
  assert(ret > 0);
  return ret - 1;
}

Point get_world_coord_from_distance(float d, track &t) {
  sector &s = t.sectors[get_sector_from_distance(d, t)];
  Point ret;
  if (s.angle == 0.f) {
    ret.x = s.starting_point_or_center.x + d * cos(s.starting_pendence);
    ret.y = s.starting_point_or_center.y + d * sin(s.starting_pendence);
  } else {
    float track_angle = s.angle * d / s.length;
    Point xt = {cos(M_PIf - track_angle), sin(track_angle)};
    ret = track_to_world(xt, s.starting_pendence, s.starting_point_or_center);
  }
  return ret;
}
