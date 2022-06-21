#include "track_loader.h"
#include <fstream>
#include <sstream>

using Point = std::pair<float, float>;

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

    Point start_sector = {0, 0};
    for (unsigned int i = 0; i < number_of_sectors; i++) {
      in.getline(buffer, 64);
      stringstream ss(buffer);
      float angle, length;
      ss >> angle >> length;

      Point end_sector = {start_sector.first, start_sector.second};
      printf("sector %d\n"
             "angle:  %f\n"
             "length: %f\n",
             i, angle, length);
      track.push_back({angle, length, 0., std::make_pair(0, 0)});
    }
  } catch (...) {
    return false;
  }
  in.close();
  return true;
}
