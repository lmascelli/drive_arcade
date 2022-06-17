#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include <math.h>
#include <utility>
#include <vector>

#define TRANSPARENT 0, 0, 0, 0
#define BLACK 0, 0, 0, 255
#define WHITE 255, 255, 255, 255
#define RED 255, 0, 0, 255
#define GREEN 0, 255, 0, 255
#define DARK_GREEN 0, 150, 0, 255
#define BLUE 0, 0, 255, 255
#define AZURE 60, 60, 150, 255
#define GRAY 100, 100, 100, 255

#define DRAW_POINT(x, y, c) drawPoint(renderer, x, y, c)

inline void drawPoint(SDL_Renderer *renderer, unsigned int x, unsigned int y,
                      unsigned char r, unsigned char g, unsigned char b,
                      unsigned char a) {
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderDrawPoint(renderer, x, y);
}

class Game {
public:
  Game();
  void run();

private:
  void input();
  void logic(float delta);
  void render();

  // Window variables
  unsigned int width = 160;
  unsigned int height = 100;
  unsigned int scale = 8;
  bool running = true;

  std::vector<std::pair<float, float>> vecTrack;

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  unsigned int previous = SDL_GetTicks();

  // Car variables
  float fCarPos = 0.f;
  float fDistance = 0.f;
  float fSpeed = 0.f;
  float fAcc = 500.f;
  float fKStreet = 0.002f;
  float fKClip = 0.003f;
  float fKGrass = 0.019f;
  bool moving = false;
  bool right = false;
  bool left = false;

// clang-format off
  //
  #define B    TRANSPARENT
  #define A    RED
  unsigned char car_data_straight[14 * 7 * 4] = {
      B, B, B, A, A, A, A, A, A, A, A, B, B, B,
      B, B, B, B, B, B, A, A, B, B, B, B, B, B,
      B, B, B, B, B, A, A, A, A, B, B, B, B, B,
      B, B, B, B, B, A, A, A, A, B, B, B, B, B,
      A, A, A, B, B, A, A, A, A, B, B, A, A, A,
      A, A, A, A, A, A, A, A, A, A, A, A, A, A,
      A, A, A, B, B, A, A, A, A, B, B, A, A, A,
  };

  unsigned char car_data_right[14 * 7 * 4] = {
      B, B, B, B, B, A, A, A, A, A, A, A, A, B,
      B, B, B, B, B, B, B, B, A, A, B, B, B, B,
      B, B, B, B, B, B, A, A, A, B, B, B, B, B,
      B, B, B, B, B, A, A, A, A, B, B, B, B, B,
      B, A, A, B, B, A, A, A, A, B, B, B, A, A,
      A, A, A, A, A, A, A, A, A, A, A, A, A, A,
      A, A, B, B, B, A, A, A, A, B, B, A, A, B,
  };

  unsigned char car_data_left[14 * 7 * 4] = {
      B, A, A, A, A, A, A, A, A, B, B, B, B, B,
      B, B, B, B, A, A, B, B, B, B, B, B, B, B,
      B, B, B, B, B, A, A, A, B, B, B, B, B, B,
      B, B, B, B, B, A, A, A, A, B, B, B, B, B,
      A, A, B, B, B, A, A, A, A, B, B, A, A, B,
      A, A, A, A, A, A, A, A, A, A, A, A, A, A,
      B, A, A, B, B, B, A, A, A, B, B, B, A, A,
  };
  // clang-format on
  SDL_Texture *car = nullptr;
  SDL_Texture *car_straight;
  SDL_Texture *car_right;
  SDL_Texture *car_left;

  // Track variables
  std::vector<std::pair<float, float>> track;
  unsigned int current_sector = 0;
  float current_sector_curvature = 0.f;
  float current_sector_point_curvature = 0.f;
  float current_sector_end = 0.f;
  float current_sector_traveled = 0.f;
  float current_sector_traveled_normalized = 0.f;
};

Game::Game() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(width * scale, height * scale, 0, &window,
                              &renderer);
  SDL_RenderSetScale(renderer, scale, scale);

  car_straight = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                   SDL_TEXTUREACCESS_STATIC, 14, 7);
  SDL_Surface *car_surface_straight =
      SDL_CreateRGBSurfaceWithFormat(0, 14, 7, 4, SDL_PIXELFORMAT_RGBA32);
  car_surface_straight->pixels = car_data_straight;
  car_straight = SDL_CreateTextureFromSurface(renderer, car_surface_straight);

  car_right = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                SDL_TEXTUREACCESS_STATIC, 14, 7);
  SDL_Surface *car_surface_rigth =
      SDL_CreateRGBSurfaceWithFormat(0, 14, 7, 4, SDL_PIXELFORMAT_RGBA32);
  car_surface_rigth->pixels = car_data_right;
  car_right = SDL_CreateTextureFromSurface(renderer, car_surface_rigth);

  SDL_Surface *car_surface_left =
      SDL_CreateRGBSurfaceWithFormat(0, 14, 7, 4, SDL_PIXELFORMAT_RGBA32);
  car_surface_left->pixels = car_data_left;
  car_left = SDL_CreateTextureFromSurface(renderer, car_surface_left);
  track.push_back(std::make_pair(0.f, 500.f));
  track.push_back(std::make_pair(90.f, 900.f));
  track.push_back(std::make_pair(0.f, 300.f));
  track.push_back(std::make_pair(70.f, 900.f));
  track.push_back(std::make_pair(50.f, 500.f));
  track.push_back(std::make_pair(-120.f, 500.f));
  track.push_back(std::make_pair(0.f, 900.f));
  track.push_back(std::make_pair(60.f, 500.f));
  track.push_back(std::make_pair(0.f, 500.f));
  track.push_back(std::make_pair(60.f, 900.f));
  track.push_back(std::make_pair(0.f, 600.f));
  track.push_back(std::make_pair(100.f, 600.f));

  current_sector_curvature = track[current_sector].first;
  current_sector_end = track[current_sector].second;
}

void Game::run() {
  while (running) {
    input();
    unsigned int now = SDL_GetTicks();
    logic(1e-3 * (float)(now - previous) / 1000);
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
        moving = true;
        break;
      case SDLK_RIGHT:
        right = true;
        break;
      case SDLK_LEFT:
        left = true;
        break;
      }
      break;
    case SDL_KEYUP:
      switch (e.key.keysym.sym) {
      case SDLK_UP:
        moving = false;
        break;
      case SDLK_RIGHT:
        right = false;
        break;
      case SDLK_LEFT:
        left = false;
        break;
      }
      break;
    default:
      break;
    }
  }
}

void Game::logic(float delta) {
  float acc;
  if (moving)
    acc = fAcc;
  else
    acc = 0.f;
  float k;
  float carPos = abs(fCarPos);

  if (carPos > 0.8)
    k = fKGrass;
  else if (carPos > 0.60)
    k = fKClip;
  else
    k = fKStreet;

  fSpeed += acc - k * fSpeed;
  fDistance += fSpeed * delta;
  current_sector_traveled += fSpeed * delta;
  current_sector_traveled_normalized =
      current_sector_traveled / current_sector_end;

  if (current_sector_traveled >= current_sector_end) {
    current_sector++;
    current_sector %= track.size();
    current_sector_traveled = 0.f;
    current_sector_curvature = track[current_sector].first;
    current_sector_end = track[current_sector].second;
  }

  current_sector_point_curvature =
      0.7 * sin(current_sector_traveled_normalized * 3.14) *
      current_sector_traveled_normalized *
      sin(current_sector_curvature / 180 * 3.14);

  const float L = fAcc / 500.f;

  float fAccCent =
      fSpeed * fSpeed * sin(current_sector_point_curvature / 180 * 3.14) * L;
  fCarPos -= fAccCent * delta * delta;
  if (right and not left)
    fCarPos += fSpeed * 1e-3 * delta;
  if (left and not right)
    fCarPos -= fSpeed * 1e-3 * delta;
  if (fCarPos > 1.f)
    fCarPos = 1.f;
  if (fCarPos < -1.f)
    fCarPos = -1.f;
}

void Game::render() {
  // CLEAR RENDERER
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // DRAW STREET
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height / 2; y++) {
      int yd = height / 2 + y;
      float fPerspective = (float)y / ((float)height / 2.f); // normalize y
      float fRoadWidth = 0.05f + 0.4f * fPerspective;
      float fClipWidth = fRoadWidth * 0.30f;
      float fCurvatureRow =
          current_sector_point_curvature * pow(1 - fPerspective, 3);
      float fMiddlePoint = 0.5f + fCurvatureRow;

      int nLeftGrass = (fMiddlePoint - fRoadWidth - fClipWidth) * width;
      int nLeftClip = (fMiddlePoint - fRoadWidth) * width;
      int nRightClip = (fMiddlePoint + fRoadWidth) * width;
      int nRightGrass = (fMiddlePoint + fRoadWidth + fClipWidth) * width;

      if (x >= 0 && x < nLeftGrass) {
        if (sin(20.f * powf(1.f - fPerspective, 3) + 0.1f * fDistance) > 0.0f)
          DRAW_POINT(x, yd, GREEN);
        else
          DRAW_POINT(x, yd, DARK_GREEN);
      } else if (x >= nLeftGrass && x < nLeftClip) {
        if (sin(50.f * powf(1.f - fPerspective, 3) + 0.1f * fDistance) > 0.0f)
          DRAW_POINT(x, yd, RED);
        else
          DRAW_POINT(x, yd, WHITE);
      } else if (x >= nLeftClip && x < nRightClip) {
        DRAW_POINT(x, yd, GRAY);
      } else if (x >= nRightClip && x < nRightGrass) {
        if (sin(50.f * powf(1.f - fPerspective, 3) + 0.1f * fDistance) > 0.0f)
          DRAW_POINT(x, yd, RED);
        else
          DRAW_POINT(x, yd, WHITE);
      } else if (x >= nRightGrass && x < width) {
        if (sin(20.f * powf(1.f - fPerspective, 3) + 0.1f * fDistance) > 0.0f)
          DRAW_POINT(x, yd, GREEN);
        else
          DRAW_POINT(x, yd, DARK_GREEN);
      }
    }
  }

  // DRAW CAR
  int nCarPos = width / 2 + ((int)(width * fCarPos / 2)) - 7;
  SDL_Rect car_src = {0, 0, 14, 7};
  SDL_Rect car_pos = {nCarPos, 80, 14, 7};
  if (right and not left)
    car = car_right;
  else if (left and not right)
    car = car_left;
  else
    car = car_straight;
  // DRAW SKY
  SDL_RenderCopy(renderer, car, &car_src, &car_pos);

  SDL_Rect sky_rect = {0, 0, static_cast<int>(width),
                       static_cast<int>(height / 2)};
  SDL_SetRenderDrawColor(renderer, AZURE);
  SDL_RenderFillRect(renderer, &sky_rect);

  SDL_RenderPresent(renderer);
}

int main(int argc, char **argv) {
  Game g;
  g.run();
  return 0;
}
