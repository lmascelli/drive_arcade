#include <SDL2/SDL.h>
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
 *                                     MAIN
 ******************************************************************************/

int main(int argc, char **argv) {
  std::vector<sector> track;
  read_track_file("../track_sample.track", track);
  //  Game g;
  //  g.run();
  return 0;
}
