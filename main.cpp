#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <vector>


#define TRANSPARENT 0, 0, 0, 0
#define BLACK 0, 0, 0, 255
#define WHITE 255, 255, 255, 255
#define RED 255, 0, 0, 255
#define GREEN 0, 255, 0, 255
#define DARK_GREEN 0, 150, 0, 255
#define BLUE 0, 0, 255, 255

class Game {
 public:
  Game() {
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

    vecTrack.push_back(std::make_pair(0.0f, 10.0f));
    vecTrack.push_back(std::make_pair(0.0f, 200.0f));
    vecTrack.push_back(std::make_pair(1.0f, 200.0f));
    vecTrack.push_back(std::make_pair(0.0f, 400.0f));
    vecTrack.push_back(std::make_pair(-1.0f, 100.0f));
    vecTrack.push_back(std::make_pair(0.0f, 200.0f));
    vecTrack.push_back(std::make_pair(-1.0f, 200.0f));
    vecTrack.push_back(std::make_pair(1.0f, 200.0f));
    vecTrack.push_back(std::make_pair(0.0f, 200.0f));
    vecTrack.push_back(std::make_pair(0.2f, 500.0f));
    vecTrack.push_back(std::make_pair(0.0f, 200.0f));
  }

  void run() {
    while (running) {
      event();
      unsigned int now = SDL_GetTicks();
      logic(1e-3 * (float)(now - previous));
      previous = now;
      render();
    }
  }

 private:
  void event() {
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
  void logic(float delta) {
    if (moving) fDistance += 200.f * delta;
  }
  void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (unsigned int x = 0; x < width; x++) {
      for (unsigned int y = 0; y < height / 2; y++) {
        float fPerspective = (float)y / ((float)height / 2.f);
        float fRoadWidth = 0.1f + 0.8f * fPerspective;
        float fClipWidth = fRoadWidth * 0.15f;
        fRoadWidth *= 0.5f;
        float fMiddlePoint = 0.5f;

        int nLeftGrass = (fMiddlePoint - fRoadWidth - fClipWidth) * width;
        int nLeftClip = (fMiddlePoint - fRoadWidth) * width;
        int nRightClip = (fMiddlePoint + fRoadWidth) * width;
        int nRightGrass = (fMiddlePoint + fRoadWidth + fClipWidth) * width;

        int nRow = height / 2 + y;

        if (x >= 0 && x < nLeftGrass) { // LEFT GRASS
          if (sin(20.f * powf(1.f - fPerspective, 3) + 0.1f * fDistance) > 0.0f)
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
          else
            SDL_SetRenderDrawColor(renderer, DARK_GREEN);
          SDL_RenderDrawPoint(renderer, x, nRow);
        } else if (x >= nLeftGrass && x < nLeftClip) { // LEFT CLIP
          if (sin(80.f * powf(1.f - fPerspective, 3) + 0.1f * fDistance) > 0.0f)
            SDL_SetRenderDrawColor(renderer, WHITE);
          else
            SDL_SetRenderDrawColor(renderer, RED);
          SDL_RenderDrawPoint(renderer, x, nRow);
        } else if (x >= nLeftClip && x < nRightClip) { // ROAD 
          SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
          SDL_RenderDrawPoint(renderer, x, nRow);
        } else if (x >= nRightClip && x < nRightGrass) { // RIGHT CLIP
          if (sin(80.f * powf(1.f - fPerspective, 3) + 0.1f * fDistance) > 0.0f)
            SDL_SetRenderDrawColor(renderer, WHITE);
          else
            SDL_SetRenderDrawColor(renderer, RED);
          SDL_RenderDrawPoint(renderer, x, nRow);
        } else if (x >= nRightGrass && x < width) { // RIGHT GRASS
          if (sin(20.f * powf(1.f - fPerspective, 3) + 0.1f * fDistance) > 0.0f)
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
          else
            SDL_SetRenderDrawColor(renderer, DARK_GREEN);
          SDL_RenderDrawPoint(renderer, x, nRow);
        }
      }
    }

    int nCarPos = width / 2 + ((int)(width * fCarPos / 2)) - 7;
    SDL_Rect car_src = {0, 0, 14, 7};
    SDL_Rect car_pos = {nCarPos, 80, 14, 7};
    SDL_RenderCopy(renderer, car, &car_src, &car_pos);
    SDL_RenderPresent(renderer);
  }

  unsigned int width = 160;
  unsigned int height = 100;
  unsigned int scale = 8;

  bool running = true;

  float fCarPos = 0.f;
  float fDistance = 0.f;

  std::vector<std::pair<float, float>> vecTrack;

  bool moving = false;

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
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

  float fOffset = 0;
  int nTrackSection = 0;

  unsigned int previous = SDL_GetTicks();
};

int main(int argc, char **argv) {
  Game g;
  g.run();
  return 0;
}
