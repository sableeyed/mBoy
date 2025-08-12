#include "cartridge.h"

#include <string.h>

cartridge_t *load_cartridge(char *filename) {
    cartridge_t *c = NULL;
    FILE *file = NULL;

    file = fopen(filename, "rb");
    if(file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    uint32_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if(size > 0) {
        c = (cartridge_t *) malloc(sizeof(cartridge_t));
        if(c == NULL) {
            fprintf(stderr, "Failed to allocate memory for cartridge\n");
            fclose(file);
            return NULL;
        }

        c->rom = (uint8_t *) malloc(sizeof(uint8_t) * size);
        c->size = size;
        if(fread(c->rom, 1, size, file) != size) {
            fprintf(stderr, "Failed to load cartridge\n");
            free(c->rom);
            free(c);
            fclose(file);
            return NULL;
        }

        c->title = (uint8_t *) strdup(filename);
    }
    fclose(file);
    return c;
}

void free_cartridge(cartridge_t *cartridge) {
    if(cartridge) {
        free(cartridge->rom);
        free(cartridge->title);
        free(cartridge);
    }
}