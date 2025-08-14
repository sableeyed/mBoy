#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include "io.h"
#include "cartridge.h"
#include "memview.h"
#include "cpu.h"
#include "display.h"

#define ID_OPEN 1
#define ID_ABOUT 2
#define ID_EXIT 3

cartridge_t *cart;
cpu_t cpu;
SDL_Window *window;
SDL_Renderer *renderer;
bool done = false;

int main(int argc, char **argv) {
    if(AllocConsole()) {
        FILE *dummy;
        freopen_s(&dummy, "CONOUT$", "w", stdout);
        freopen_s(&dummy, "CONOUT$", "w", stderr);
    }

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("MasochistBoy", 640, 480, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, NULL);
    init_display(renderer);
    

    if(NULL == window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    /*
     * not portable which kinda defeats the purpose of SDL and cross platform..
    */

    HWND mboyHwnd = FindWindow(NULL, "MasochistBoy");

    if(mboyHwnd) {
        HMENU hMenuBar = CreateMenu();
        HMENU hMenu = CreateMenu();
        AppendMenu(hMenu, MF_STRING, ID_OPEN, "Open");
        AppendMenu(hMenu, MF_STRING, ID_ABOUT, "About");
        AppendMenu(hMenu, MF_STRING, ID_EXIT, "Exit");
        AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR) hMenu, "File");
        SetMenu(mboyHwnd, hMenuBar);
        SDL_RaiseWindow(window);
    }

    MSG msg;
    while(!done) {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_QUIT) {
                done = true;
            }
            else if(msg.message == WM_COMMAND) {
                switch(LOWORD(msg.wParam)) {
                    case ID_OPEN:
                        char *path = get_rom(mboyHwnd);
                        cart = load_cartridge(path);
                        free(path);
                        cpu_init(&cpu, cart);
                        cpu.hwnd = mboyHwnd;
                        break;
                    case ID_ABOUT:
                        printf("About MasochistBoy\n");
                        MessageBox(mboyHwnd, "MasochistBoy", "About", MB_OK | MB_ICONINFORMATION);
                        break;
                    case ID_EXIT:
                        done = true;
                        break;
                }
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        display_renderer(renderer);
        SDL_RenderPresent(renderer);

        if(cart) {
            cpu_step(&cpu);
        }
        SDL_Delay(1);
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    free_cartridge(cart);

    return 0;
}