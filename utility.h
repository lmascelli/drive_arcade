#pragma once
#include <SDL2/SDL.h>

inline void drawPoint(SDL_Renderer *renderer, unsigned int x, unsigned int y,
                      unsigned char r, unsigned char g, unsigned char b,
                      unsigned char a);

#define DRAW_POINT(x, y, c) drawPoint(renderer, x, y, c)

typedef struct {
  float s;
  float view_angle;
} T_Struct;

inline SDL_Point track_to_world(T_Struct);
inline T_Struct world_to_track(SDL_Point x);
