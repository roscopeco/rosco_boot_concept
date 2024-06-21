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

static SDL_Color colors[] = {
    { 0x00, 0x00, 0x00, 0xff },     // COLOR_BLACK
    { 0xdc, 0xdc, 0xdc, 0xff },     // COLOR_WHITE
    { 0xdc, 0xdc, 0x00, 0xff },     // COLOR_YELLOW
    { 0x2f, 0x3c, 0x48, 0xff },     // COLOR_BACKGROUND
    { 0x1f, 0x2c, 0x38, 0xff },     // COLOR_BACKGROUND_SHADOW
    { 0xcc, 0xcc, 0xcc, 0xff },     // COLOR_WINDOW_BACKGROUND
    { 0x10, 0x80, 0xa0, 0xff },     // COLOR_SELECTION_BAR
    { 0x10, 0x10, 0x10, 0xff },     // COLOR_ITEM_TEXT
    { 0xe6, 0xe6, 0xe6, 0xff },     // COLOR_ITEM_HIGHLIGHT_TEXT
    { 0x00, 0x00, 0x00, 0xff },     // <unused>
    { 0x00, 0x00, 0x00, 0xff },     //   ...
    { 0x00, 0x00, 0x00, 0xff },     //   ...
    { 0x00, 0x00, 0x00, 0xff },     //   ...
    { 0x00, 0x00, 0x00, 0xff },     //   ...
    { 0x00, 0x00, 0x00, 0xff },     //   ...
    { 0x00, 0x00, 0x00, 0xff },     //   ...
};

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

void backend_set_color(BACKEND_COLOR color) {
    SDL_Color sdl_color = colors[color];
    SDL_SetRenderDrawColor(renderer, sdl_color.r, sdl_color.g, sdl_color.b, sdl_color.a);
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
