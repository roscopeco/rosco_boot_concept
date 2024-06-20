#include <stdbool.h>
#include <sys/time.h>

#include <SDL2/SDL.h>

#include "view.h"
#include "backend.h"

#if VIEW_HRES == 320
#define VIEW_HSCALE 4.0
#define VIEW_VSCALE 4.0
#elif VIEW_HRES == 640
#define VIEW_HSCALE 2.0
#define VIEW_VSCALE 2.0
#else
#error Unknown resolution - add scale factor to sdl_backend.c
#endif

static SDL_Window *window;
static SDL_Renderer *renderer;

bool backend_init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("The Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VIEW_HRES * VIEW_HSCALE, VIEW_VRES * VIEW_VSCALE, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetScale(renderer, VIEW_HSCALE, VIEW_VSCALE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return true;
}

void backend_clear() {
    SDL_RenderClear(renderer);
}

void backend_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void backend_draw_pixel(int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

BACKEND_EVENT backend_poll_event() {
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return QUIT;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_DOWN:
            case SDLK_s:
                return DOWN;

            case SDLK_UP:
            case SDLK_w:
                return UP;

            case SDLK_ESCAPE:
                return QUIT;
            }
        default:
            return NONE;
        }
    }

    return NONE;
}

uint32_t backend_get_ticks() {
    struct timeval te; 
    gettimeofday(&te, NULL);
    return te.tv_sec*100 + te.tv_usec/10000; // calculate 1/100th ticks
}

void backend_draw_rect(Rect *rect) {
    SDL_Rect sdl_rect = { .x = rect->x, .y = rect->y, .w = rect->w, .h = rect->h };
    SDL_RenderDrawRect(renderer, &sdl_rect);
}

void backend_fill_rect(Rect *rect) {
    SDL_Rect sdl_rect = { .x = rect->x, .y = rect->y, .w = rect->w, .h = rect->h };
    SDL_RenderFillRect(renderer, &sdl_rect);
}

void backend_present() {
    SDL_RenderPresent(renderer);
}

void backend_done() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
