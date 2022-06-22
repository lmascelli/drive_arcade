#include <SDL2/SDL.h>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <utility>
#include <vector>

#include "car.h"
#include "defines.h"
#include "track.h"
#include "utility.h"

class Game {
public:
  Game();
  void run();

private:
  void input();
  void logic(float delta); // in seconds
  void render();

  // Window variables
  unsigned int width = 160;
  unsigned int height = 100;
  unsigned int scale = 8;
  bool running = true;

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  unsigned int previous = SDL_GetTicks();

  // Camera varibles
  float horizont_distance = 300.; // meters
  float fov_angle = M_PI_2f;      // radians
  float rotation_angle = 0.f;
  SDL_FPoint offset = {0.f, 0.f};

  // Car render variables
  SDL_Texture *car = nullptr;
  SDL_Texture *car_straight;
  SDL_Texture *car_right;
  SDL_Texture *car_left;
};

Game::Game() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(width * scale, height * scale, 0, &window,
                              &renderer);
  SDL_RenderSetScale(renderer, scale, scale);

  /*------------------------------ car textures ------------------------------*/
  SDL_Surface *car_surface_straight =
      SDL_CreateRGBSurfaceWithFormat(0, 14, 7, 4, SDL_PIXELFORMAT_RGBA32);
  car_surface_straight->pixels = car_data_straight;
  car_straight = SDL_CreateTextureFromSurface(renderer, car_surface_straight);

  SDL_Surface *car_surface_rigth =
      SDL_CreateRGBSurfaceWithFormat(0, 14, 7, 4, SDL_PIXELFORMAT_RGBA32);
  car_surface_rigth->pixels = car_data_right;
  car_right = SDL_CreateTextureFromSurface(renderer, car_surface_rigth);

  SDL_Surface *car_surface_left =
      SDL_CreateRGBSurfaceWithFormat(0, 14, 7, 4, SDL_PIXELFORMAT_RGBA32);
  car_surface_left->pixels = car_data_left;
  car_left = SDL_CreateTextureFromSurface(renderer, car_surface_left);
}

void Game::run() {
  while (running) {
    input();
    unsigned int now = SDL_GetTicks();
    logic(1e-3 * (float)(now - previous));
    previous = now;
    render();
  }
}

void Game::input() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      running = false;
      break;
    case SDL_KEYDOWN:
      switch (e.key.keysym.sym) {
      case SDLK_UP:
        break;
      case SDLK_DOWN:
        break;
      case SDLK_RIGHT:
        break;
      case SDLK_LEFT:
        break;
      }
      break;
    case SDL_KEYUP:
      switch (e.key.keysym.sym) {
      case SDLK_UP:
        break;
      case SDLK_DOWN:
        break;
      case SDLK_RIGHT:
        break;
      case SDLK_LEFT:
        break;
      }
      break;
    default:
      break;
    }
  }
}

void Game::logic(float delta) {}

void Game::render() {
  /*----------------------------- clear renderer -----------------------------*/
  SDL_SetRenderDrawColor(renderer, BLACK);
  SDL_RenderClear(renderer);

  /*------------------------------- draw street ------------------------------*/
  for (unsigned int row = 0; row < height / 2; row++) {
    float y = row / (height / 2.) * horizont_distance;
    float road_x;
    for (unsigned int col = 0; col < width; col++) {
    }
  }

  /*-------------------------------- draw car --------------------------------*/

  /*-------------------------------- draw sky --------------------------------*/

  SDL_Rect sky_rect = {0, 0, static_cast<int>(width),
                       static_cast<int>(height / 2)};
  SDL_SetRenderDrawColor(renderer, AZURE);
  SDL_RenderFillRect(renderer, &sky_rect);

  /*----------------------------- present render -----------------------------*/
  SDL_RenderPresent(renderer);
}

/******************************************************************************
 *                                    TEST
 ******************************************************************************/

void test() {
  SDL_Window *test_wind;
  SDL_Renderer *test_rend;
  bool running = true;

  const unsigned int width = 100, height = 200;
  track t;
  read_track_file("../track_sample.track", t);

  float d = 0.f;
  unsigned char pixels[width * height * 4];
  while (d < t.length - 1) {
    Point x = get_world_coord_from_distance(d, t);
    assert(x.x < width and x.y < height);
    for (unsigned int i = 0; i < 4; i++)
      pixels[static_cast<unsigned int>(x.x) +
             width * static_cast<unsigned int>(x.y) + i] = 0;
    d++;
  }

  if (!SDL_CreateWindowAndRenderer(300, 600, SDL_WINDOW_SHOWN, &test_wind,
                                   &test_rend)) {
    SDL_RenderSetScale(test_rend, 2, 2);
    SDL_Surface *test_surf;

    test_surf = SDL_CreateRGBSurfaceWithFormatFrom(
        (void *)pixels, width, height, 8, 0, SDL_PIXELFORMAT_RGBA32);
  }

  while (running) {

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        running = false;
        break;
      }
    }

    SDL_SetRenderDrawColor(test_rend, WHITE);
    SDL_RenderClear(test_rend);
    SDL_RenderPresent(test_rend);
  }
}

/******************************************************************************
 *                                     MAIN
 ******************************************************************************/

int main(int argc, char **argv) {
  test();
  //  Game g;
  //  g.run();
  return 0;
}
