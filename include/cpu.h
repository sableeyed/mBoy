#ifndef CPU_H
#define CPU_H

#define CPU_ZERO_FLAG_BIT 7 //z
#define CPU_SUBTRACTION_FLAG_BIT 6 //n
#define CPU_HALF_CARRY_FLAG_BIT 5 //h
#define CPU_CARRY_FLAG_BIT 4 //c

#include "registers.h"
#include "cartridge.h"
#include <windows.h>

typedef struct cpu_t {
    uint8_t memory[0x10000];
    HWND hwnd; //do we need this?
} cpu_t;

void cpu_init(cpu_t *cpu, cartridge_t *cart);
uint8_t cpu_read8(cpu_t *cpu, uint16_t addr);
void cpu_write8(cpu_t *cpu, uint16_t addr, uint8_t value);
void cpu_step(cpu_t *cpu);

void cpu_unimplemented(cpu_t *cpu, uint8_t opcode);

#endif