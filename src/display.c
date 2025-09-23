#include "display.h"
#include <stdio.h>

COLOR framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
static SDL_Texture *texture = NULL;

void init_display(SDL_Renderer *renderer) {
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if(!texture) {
        SDL_Log("Failed to create textureL: %s", SDL_GetError());
    }

    for(int x = 0; x < SCREEN_WIDTH*SCREEN_HEIGHT; x++) {
        framebuffer[x].r = 255;
        framebuffer[x].g = 255;
        framebuffer[x].b = 255;
    }
}

void display_renderer(SDL_Renderer *renderer) {
    if(!texture) { return; }
    SDL_UpdateTexture(texture, NULL, framebuffer, SCREEN_WIDTH * sizeof(COLOR));

    int windowW, windowH;
    SDL_GetCurrentRenderOutputSize(renderer, &windowW, &windowH);

    // float scale = (float) windowW / SCREEN_WIDTH;
    // if((float)windowH / SCREEN_HEIGHT < scale) {
    //     scale = (float)windowH / SCREEN_HEIGHT;
    // }

    float scaleX = (float)windowW / SCREEN_WIDTH;
    float scaleY = (float)windowH / SCREEN_HEIGHT;
    float scale = scaleX < scaleY ? scaleX : scaleY;

    int destW = (int)(SCREEN_WIDTH * scale);
    int destH = (int)(SCREEN_HEIGHT * scale);
    SDL_FRect destRect = {
        (windowW - destW) / 2.0f,
        (windowH - destH) / 2.0f,
        (float)destW,
        (float)destH
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderTexture(renderer, texture, NULL, &destRect);

    SDL_RenderPresent(renderer);

}
