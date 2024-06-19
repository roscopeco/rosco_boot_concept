#include <stdbool.h>
#include <sys/time.h>

#include <SDL2/SDL.h>

#include "view.h"
#include "backend.h"

#define VIEW_HSCALE 2.0
#define VIEW_VSCALE 2.0

static SDL_Window *window;
static SDL_Renderer *renderer;

bool backend_init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(VIEW_HRES * VIEW_HSCALE, VIEW_VRES * VIEW_VSCALE, 0, &window, &renderer);
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

void backend_draw_text(const char *str, int x, int y, const uint8_t *font, int font_height) {
    unsigned char c;

    while ((c = *str++)) {
        const uint8_t *font_ptr = font + (c * font_height);

        for (int dy = 0; dy < font_height; dy++) {
            for (int dx = 0; dx < 8; dx++) {
                if ((*font_ptr & (1 << (8-dx))) != 0) {
                    SDL_RenderDrawPoint(renderer, x+dx, y+dy);
                }
            
            }

            font_ptr++;
        }

        x += 8;
    }
}

void backend_present() {
    SDL_RenderPresent(renderer);
}

void backend_done() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
