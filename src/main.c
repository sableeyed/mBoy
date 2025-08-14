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

#define ID_OPEN 1
#define ID_ABOUT 2
#define ID_EXIT 3

cartridge_t *cart;

static void show_nintendo_boot(SDL_Window* win, int ms);

int main(int argc, char **argv) {
    if(AllocConsole()) {
        FILE *dummy;
        freopen_s(&dummy, "CONOUT$", "w", stdout);
        freopen_s(&dummy, "CONOUT$", "w", stderr);
    }

    SDL_Window *window;
    bool done = false;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("MasochistBoy", 640, 480, SDL_WINDOW_OPENGL);

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
    
    show_nintendo_boot(window, 1500);
    cpu_t cpu;

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

/*
 * Get rid of this once we actually implement graphics
*/
static void show_nintendo_boot(SDL_Window* win, int ms) {
    // Fill window white using GDI, then draw the text "Nintendo"
    SDL_PropertiesID props = SDL_GetWindowProperties(win);
    //HWND hwnd = (HWND)SDL_GetProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
    HWND hwnd = (HWND) SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
    if (!hwnd) return;

    RECT rc; GetClientRect(hwnd, &rc);
    HDC hdc = GetDC(hwnd);

    HBRUSH white = CreateSolidBrush(RGB(255,255,255));
    FillRect(hdc, &rc, white);
    DeleteObject(white);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0,0,0));

    // Big, bold, centered-ish
    HFONT font = CreateFontA(
        (rc.bottom - rc.top) / 5, 0, 0, 0, FW_BOLD,
        FALSE, FALSE, FALSE, ANSI_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, "Arial");
    HFONT old = (HFONT)SelectObject(hdc, font);

    const char* txt = "Nintendo";
    SIZE sz; GetTextExtentPoint32A(hdc, txt, (int)strlen(txt), &sz);
    int x = (rc.right - rc.left - sz.cx)/2;
    int y = (rc.bottom - rc.top - sz.cy)/2;
    TextOutA(hdc, x, y, txt, (int)strlen(txt));

    SelectObject(hdc, old);
    DeleteObject(font);
    ReleaseDC(hwnd, hdc);

    // Keep window responsive while we "display the logo"
    Uint64 start = SDL_GetTicks();
    SDL_Event e;
    while ((int)(SDL_GetTicks() - start) < ms) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) return;
        }
        SDL_Delay(10);
    }
}
