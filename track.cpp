#include "track.h"
#include "utility.h"
#include <fstream>
#include <sstream>

bool read_track_file(std::string filename, std::vector<sector> &track) {
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
    printf("reading %d sectors\n", number_of_sectors);

    float sector_starting_pendence = 0;
    Point sector_starting_point = {0.f, 0.f};

    for (unsigned int i = 0; i < number_of_sectors; i++) {
      in.getline(buffer, 64);
      stringstream ss(buffer);
      float angle, length;
      ss >> angle >> length;

      printf("sector %d\n"
             "angle:  %f\n"
             "length: %f\n",
             i, angle, length);

      float radius = angle == 0.f ? 0.f : (length / angle);

      if (radius != 0.f) {
        Point local_center = {(angle > 0.f ? 1.f : -1.f) * radius, 0.f};
        Point world_center = track_to_world(local_center, sector_starting_pendence);
      } else {
        std::function<float(float)> f = [sector_starting_pendence,
                                         sector_starting_point](float x) {
          return tan(sector_starting_pendence) * (x - sector_starting_point.x) +
                 sector_starting_point.y;
        };
        track.push_back({angle, length, f});
      }
    }
  } catch (...) {
    return false;
  }
  in.close();
  return true;
}
