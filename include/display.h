#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <SDL3/SDL.h>

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

struct rgb {
    uint8_t r, g, b;
};
typedef struct rgb COLOR;

extern COLOR framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

extern const COLOR palette[4];

void draw_frame_buffer(SDL_Renderer *renderer);
void render_scanline(SDL_Renderer *renderer);
void init_display(SDL_Renderer *renderer);
void display_renderer(SDL_Renderer *renderer);

#endif