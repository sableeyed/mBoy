#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

uint8_t mem_read8(uint16_t addr);
void mem_write8(uint16_t addr, uint8_t value);
void mem_init(uint8_t *rom, size_t size, uint8_t *boot);

#endif