#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include <math.h>
#include <utility>
#include <iostream>
#include <vector>


/******************************************************************************
*                                 DEFINES
******************************************************************************/

#define MACHINE_ZERO 1e-9

#define COLOR(X)  (X >> 24) & 0x000000ff ,\
                  (X >> 16) & 0x000000ff ,\
                  (X >>  8) & 0x000000ff ,\
                  (X      ) & 0x000000ff

#define TRANSPARENT COLOR(0x00000000)
#define BLACK       COLOR(0x000000ff)
#define WHITE       COLOR(0xffffffff)
#define RED         COLOR(0xff0000ff)
#define GREEN       COLOR(0x00ff00ff) 
#define DARK_GREEN  COLOR(0x00aa00ff)
#define BLUE        COLOR(0x0000ffff)
#define AZURE       COLOR(0x5050ffff)
#define GRAY        COLOR(0xaaaaaaff) 

/******************************************************************************
*                           UTILITY FUNCTIONS 
******************************************************************************/

inline void drawPoint(SDL_Renderer *renderer, unsigned int x, unsigned int y,
                      unsigned char r, unsigned char g, unsigned char b,
                      unsigned char a) {
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderDrawPoint(renderer, x, y);
}

#define DRAW_POINT(x, y, c) drawPoint(renderer, x, y, c)

/*****************************************************************************/

class Game {
public:
  Game();
  void run();

private:
  void input();
  void logic(float delta);    // in seconds
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

  // Car parameters
  float fCarPos = 0.f;
  float current_distance = 0.f;
  float current_speed = 0.f;
  float fHSpeed = 0.f;
  
  float fAcc = 4.e2f;         // m/s^2
  float fKStreet = 1.e-1f;    // 1/s
  float fKClip = 1.5e-1f;     // 1/s
  float fKGrass = 2.e-1f;     // 1/s
  
  // Car state variables
  bool accelerating = false;
  bool right = false;
  bool left = false;

  // Car render variables
  SDL_Texture *car = nullptr;
  SDL_Texture *car_straight;
  SDL_Texture *car_right;
  SDL_Texture *car_left;

  // clang-format off
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
  

  // Track variables
  std::vector<std::pair<float, float>> track;
  unsigned int current_sector = 0;
  float current_sector_total_angle = 0.f;
  float current_sector_point_curvature = 0.f;
  float current_sector_length = 0.f;
  float current_sector_radius = 0.f;
  float current_sector_traveled = 0.f;
  float current_sector_traveled_normalized = 0.f;
};

/******************************************************************************
*                                 INITIALIZATION
******************************************************************************/

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

  current_sector_total_angle = track[current_sector].first;
  current_sector_length = track[current_sector].second;
  current_sector_radius = (current_sector_total_angle > MACHINE_ZERO) ?
                           current_sector_length/current_sector_total_angle :
                           0.f;
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
        accelerating = true;
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
        accelerating = false;
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
  
/******************************************************************************
*                                 CAR MOVEMENT 
******************************************************************************/
/*--------------------------- Setting parameters ----------------------------*/ 

  float current_acc;
  if (accelerating)
    current_acc = fAcc;
  else
    current_acc = 0.f;
  float current_k;
  float car_pos_abs = abs(fCarPos);

  if (car_pos_abs > 0.8)
    current_k = fKGrass;
  else if (car_pos_abs > 0.60)
    current_k = fKClip;
  else
    current_k = fKStreet;

/*----------------------- Computing distance state --------------------------*/
/*
 * dv/dt = acc - kv
 * ds/dt = v
 * 
 * v = current_speed
 * acc = current_acc
 * s = current_distance
 */

  current_speed += (current_acc - current_k * current_speed) * delta;
  printf("%f\n", current_k * current_speed);
  float current_distance_step = current_speed * delta;
  current_distance += current_distance_step;


/*------------------------- Managing track sector ---------------------------*/

  current_sector_traveled += current_distance_step;
  current_sector_traveled_normalized =
      current_sector_traveled / current_sector_length;

  if (current_sector_traveled >= current_sector_length) {
    current_sector++;
    current_sector %= track.size();
    current_sector_traveled = 0.f;
    current_sector_total_angle = track[current_sector].first;
    current_sector_length = track[current_sector].second;
    current_sector_radius = (current_sector_total_angle > MACHINE_ZERO) ? current_sector_length/current_sector_total_angle : 0.f;
  }

/*-------------------- Computing car horizontal state -----------------------*/


}

void Game::render() {
  // CLEAR RENDERER
  SDL_SetRenderDrawColor(renderer, BLACK);
  SDL_RenderClear(renderer);

  // DRAW STREET
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

    for (int x = 0; x < width; x++) {
      if (x >= 0 && x < nLeftGrass) {
        if (sin(20.f * powf(1.f - fPerspective, 3) + 0.1f * current_distance) > 0.0f)
          DRAW_POINT(x, yd, GREEN);
        else
          DRAW_POINT(x, yd, DARK_GREEN);
      } else if (x >= nLeftGrass and x < nLeftClip) {
        if (sin(50.f * powf(1.f - fPerspective, 3) + 0.1f * current_distance) > 0.0f)
          DRAW_POINT(x, yd, RED);
        else
          DRAW_POINT(x, yd, WHITE);
      } else if (x >= nLeftClip and x < nRightClip) {
        DRAW_POINT(x, yd, GRAY);
      } else if (x >= nRightClip and x < nRightGrass) {
        if (sin(50.f * powf(1.f - fPerspective, 3) + 0.1f * current_distance) > 0.0f)
          DRAW_POINT(x, yd, RED);
        else
          DRAW_POINT(x, yd, WHITE);
      } else if (x >= nRightGrass and x < width) {
        if (sin(20.f * powf(1.f - fPerspective, 3) + 0.1f * current_distance) > 0.0f)
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

/******************************************************************************
*                                     MAIN
******************************************************************************/

int main(int argc, char **argv) {
  Game g;
  g.run();
  return 0;
}
