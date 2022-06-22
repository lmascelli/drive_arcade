#include "utility.h"

void drawPoint(SDL_Renderer *renderer, unsigned int x, unsigned int y,
                      unsigned char r, unsigned char g, unsigned char b,
                      unsigned char a) {
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderDrawPoint(renderer, x, y);
}


SDL_FPoint track_to_world(SDL_FPoint x, float angle,
                                 SDL_FPoint translation) {
  return {
      (x.x - translation.x) * cos(angle) - (x.y - translation.y) * sin(angle),
      (x.x - translation.x) * sin(angle) + (x.y - translation.y) * cos(angle)};
}

SDL_FPoint world_to_track(SDL_FPoint x, float angle,
                                 SDL_FPoint translation) {
  return {x.x * cos(angle) + x.y * sin(angle) + translation.x,
          x.x * -sin(angle) + x.y * cos(angle) + translation.y};
}

