#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <math.h>

#define TRANSPARENT 0, 0, 0, 0
#define BLACK 0, 0, 0, 255
#define WHITE 255, 255, 255, 255
#define RED 255, 0, 0, 255
#define GREEN 0, 255, 0, 255
#define DARK_GREEN 0, 150, 0, 255
#define BLUE 0, 0, 255, 255
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
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  unsigned int previous = SDL_GetTicks();

  // Car variables
  float fCarPos = 0.f;
  float fDistance = 0.f;
  float fSpeed = 0.f;
  float fAcc = 0.5f;
  float k = 0.002f;
  bool moving = false;
  unsigned char car_data[14 * 7 * 4] = {
      TRANSPARENT, TRANSPARENT, TRANSPARENT, BLUE,        BLUE,
      BLUE,        BLUE,        BLUE,        BLUE,        BLUE,
      BLUE,        TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT,
      TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT,
      BLUE,        BLUE,        TRANSPARENT, TRANSPARENT, TRANSPARENT,
      TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT,
      TRANSPARENT, TRANSPARENT, TRANSPARENT, BLUE,        BLUE,
      BLUE,        BLUE,        TRANSPARENT, TRANSPARENT, TRANSPARENT,
      TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT,
      TRANSPARENT, TRANSPARENT, BLUE,        BLUE,        BLUE,
      BLUE,        TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT,
      TRANSPARENT, BLUE,        BLUE,        BLUE,        TRANSPARENT,
      TRANSPARENT, BLUE,        BLUE,        BLUE,        BLUE,
      TRANSPARENT, TRANSPARENT, BLUE,        BLUE,        BLUE,
      BLUE,        BLUE,        BLUE,        BLUE,        BLUE,
      BLUE,        BLUE,        BLUE,        BLUE,        BLUE,
      BLUE,        BLUE,        BLUE,        BLUE,        BLUE,
      BLUE,        BLUE,        TRANSPARENT, TRANSPARENT, BLUE,
      BLUE,        BLUE,        BLUE,        TRANSPARENT, TRANSPARENT,
      BLUE,        BLUE,        BLUE,
  };
  SDL_Texture *car = nullptr;
};

Game::Game() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(width * scale, height * scale, 0, &window,
                              &renderer);
  SDL_RenderSetScale(renderer, scale, scale);
  car = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                          SDL_TEXTUREACCESS_STATIC, 14, 7);
  SDL_Surface *car_surface =
      SDL_CreateRGBSurfaceWithFormat(0, 14, 7, 4, SDL_PIXELFORMAT_RGBA32);
  car_surface->pixels = car_data;
  car = SDL_CreateTextureFromSurface(renderer, car_surface);
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
        moving = true;
        break;
      }
      break;
    case SDL_KEYUP:
      switch (e.key.keysym.sym) {
      case SDLK_UP:
        moving = false;
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
  fSpeed += acc - k * fSpeed;
  fDistance += fSpeed * delta;
}

void Game::render() {
  // CLEAR RENDERER
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // DRAW STREET
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height / 2; y++) {
      float fPerspective = (float)y / ((float)height / 2.f); // normalize y
      float fRoadWidth = 0.05f + 0.4f * fPerspective;
      float fClipWidth = fRoadWidth * 0.30f;
      float fMiddlePoint = 0.5f;

      int nLeftGrass = (fMiddlePoint - fRoadWidth - fClipWidth) * width;
      int nLeftClip = (fMiddlePoint - fRoadWidth) * width;
      int nRightClip = (fMiddlePoint + fRoadWidth) * width;
      int nRightGrass = (fMiddlePoint + fRoadWidth + fClipWidth) * width;

      int yd = height / 2 + y;

      if (x >= 0 && x < nLeftGrass) {
        if (sin(20.f * powf(1.f - fPerspective, 3) + 0.1f * fDistance) > 0.0f)
          DRAW_POINT(x, yd, GREEN);
        else
          DRAW_POINT(x, yd, DARK_GREEN);
      } else if (x >= nLeftGrass && x < nLeftClip) {
        DRAW_POINT(x, yd, RED);
      } else if (x >= nLeftClip && x < nRightClip) {
        DRAW_POINT(x, yd, GRAY);
      } else if (x >= nRightClip && x < nRightGrass) {
        DRAW_POINT(x, yd, RED);
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
  SDL_RenderCopy(renderer, car, &car_src, &car_pos);
  SDL_RenderPresent(renderer);
}

int main(int argc, char **argv) {
  Game g;
  g.run();
  return 0;
}
