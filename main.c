#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDITH 1000
#define HEIGHT 700
#define GRID_SIZE 2
static int grid[HEIGHT / GRID_SIZE][WIDITH / GRID_SIZE] = {0};
int type = 1;

void update() {
  for (int y = (HEIGHT / GRID_SIZE - 2); y >= 0; y--) {
    for (int x = 0; x < (WIDITH / GRID_SIZE); x++) {
      int stype = grid[y][x];
      if (grid[y][x] != 0) {
        if (grid[y + 1][x] == 0) {
          grid[y][x] = 0;
          grid[y + 1][x] = stype;

        } else if (grid[y + 1][x + 1] == 0 && grid[y + 1][x - 1] == 0) {
          if (rand() % 2) {
            grid[y][x] = 0;
            grid[y + 1][x + 1] = stype;
          } else {
            grid[y][x] = 0;
            grid[y + 1][x - 1] = stype;
          }

        } else if (grid[y + 1][x + 1] == 0) {
          grid[y][x] = 0;
          grid[y + 1][x + 1] = stype;
        } else if (grid[y + 1][x - 1] == 0) {
          grid[y][x] = 0;
          grid[y + 1][x - 1] = stype;
        }
      }
    }
  }
}
void drawGrid(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  for (int i = 0; i <= WIDITH - GRID_SIZE; i += GRID_SIZE) {
    SDL_RenderDrawLine(renderer, i, 0, i, HEIGHT);
  }
  for (int i = 0; i <= HEIGHT - GRID_SIZE; i += GRID_SIZE) {
    SDL_RenderDrawLine(renderer, 0, i, WIDITH, i);
  }
}

void addSand(int x, int y) {
  y = y / GRID_SIZE;
  x = x / GRID_SIZE;
  int ofX;
  int ofY;
  int sandQ = 20;
  for (int i = 0; i < sandQ; i++) {
    ofX = (rand() % 5) - 1;
    ofY = (rand() % 5) - 1;
    x = x + ofX;
    y = y + ofY;
    if (x >= 0 && x < WIDITH / GRID_SIZE && y >= 0 && y < HEIGHT / GRID_SIZE) {
      if (grid[y][x] == 0) {
        grid[y][x] = type;
      }
    }
  }
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("init failed");
    return 1;
  }
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_CreateWindowAndRenderer(WIDITH, HEIGHT, 0, &window, &renderer);
  int running = 1;
  SDL_Event event;
  int isDown = 0;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        isDown = 1;
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        addSand(mx, my);
      } else if (event.type == SDL_MOUSEBUTTONUP) {
        isDown = 0;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_a) {
          type += 1;
        }
        if (event.key.keysym.sym == SDLK_b) {
          type -= 1;
        }
      }
      //  else if ((event.type == SDL_MOUSEMOTION && isDown) || isDown) {
      //  int mx, my;
      //  SDL_GetMouseState(&mx, &my);
      //  addSand(mx, my);
      //}
    }
    if (isDown) {
      int mx, my;
      SDL_GetMouseState(&mx, &my);
      addSand(mx, my);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    // drawGrid(renderer);
    // SDL_SetRenderDrawColor(renderer, 194, 178, 128, 53);
    for (int y = 0; y < (HEIGHT / GRID_SIZE); y++) {
      for (int x = 0; x < (WIDITH / GRID_SIZE); x++) {
        if (grid[y][x] == 1) {
          SDL_SetRenderDrawColor(renderer, 255, 40, 160, 255);
        } else if (grid[y][x] == 2) {
          SDL_SetRenderDrawColor(renderer, 40, 180, 255, 255);
        } else if (grid[y][x] == 3) {
          SDL_SetRenderDrawColor(renderer, 255, 255, 60, 255);
        }
        if (grid[y][x] != 0) {
          SDL_Rect rect = {x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE};
          SDL_RenderFillRect(renderer, &rect);
        }
      }
    }
    for (int i = 0; i < 5; i++) {
      update();
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
