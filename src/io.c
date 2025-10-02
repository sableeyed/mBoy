#include "io.h"
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <commdlg.h>

char *get_rom(HWND hwnd) {
    char path[MAX_PATH] = {0};

    OPENFILENAME file = {0};
    file.lStructSize = sizeof(file);
    file.hwndOwner = hwnd;
    file.lpstrFile = path;
    file.nMaxFile = sizeof(path);
    file.lpstrFilter = "All Files\0*.*\0";
    file.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    file.lpstrTitle = "Select ROM";

    if(GetOpenFileName(&file)) {
        char *result = (char *)malloc(strlen(path) + 1);
        if(!result) { return NULL; }
        strcpy(result, path);
        return result;
    }
    else {
        return NULL;
    }
}