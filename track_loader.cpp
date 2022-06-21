#include "track_loader.h"

bool read_track_file(std::string filename, std::vector<sector> &track) {
  using namespace std;
  ifstream in(filename);
  if (!in.is_open())
    return false;
  try {
    unsigned int number_of_sectors;
    in >> number_of_sectors;

    printf("reading %d sectors\n", number_of_sectors);
    for (unsigned int i = 0; i < number_of_sectors; i++) {
      float angle, length, x, y;
      in >> angle >> length >> x >> y;
      printf("sector %d\n"
             "angle:  %f\n"
             "length: %f\n"
             "center: {%f, %f}\n",
             i, angle, length, x, y);
      track.push_back({angle, length, std::make_pair(x, y)});
    }
  } catch (...) {
    return false;
  }
  in.close();
  return true;
}
