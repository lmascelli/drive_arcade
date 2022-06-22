#pragma once
#include <SDL2/SDL.h>

inline void drawPoint(SDL_Renderer *renderer, unsigned int x, unsigned int y,
                      unsigned char r, unsigned char g, unsigned char b,
                      unsigned char a);

#define DRAW_POINT(x, y, c) drawPoint(renderer, x, y, c)


/**
 * @brief returns x coords in the track reference system to the global one.
 *         ----   --------------------  -------
 *         |x0|   | cos(α)   -sin(α) |  |x1-xt|
 *         |  | = |                  |* |     |
 *         |y0|   | sin(α)   cos(α)  |  |y1-yt|
 *         ----   --------------------  -------
 *
 */
SDL_FPoint track_to_world(SDL_FPoint x, float angle,
                                SDL_FPoint translation = {0.f, 0.f});
/**
 * @brief returns x coords in the track reference system to the global one.
 *         ----   ------------------- ----   ----
 *         |x1|   | cos(α)   sin(α) | |x0|   |xt|
 *         |  | = |                 |*|  | + |  | 
 *         |y1|   | -sin(α)  cos(α) | |y0|   |yt|
 *         ----   ------------------- ----   ----
 *
 */
SDL_FPoint world_to_track(SDL_FPoint x, float angle,
                                SDL_FPoint translation = {0.f, 0.f} );
