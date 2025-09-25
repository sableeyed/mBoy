#ifndef CPU_H
#define CPU_H

#define CPU_ZERO_FLAG_BIT 7 //z
#define CPU_SUBTRACTION_FLAG_BIT 6 //n
#define CPU_HALF_CARRY_FLAG_BIT 5 //h
#define CPU_CARRY_FLAG_BIT 4 //c

#include "registers.h"
#include "cartridge.h"
#include <windows.h>

/*
 * ld a, b -> ld dst, src (intel syntax)
 */

typedef struct cpu_t {
    uint8_t memory[0x10000];
    HWND hwnd; //do we need this?
} cpu_t;

void cpu_init(cpu_t *cpu, cartridge_t *cart);
uint8_t cpu_read8(cpu_t *cpu, uint16_t addr);
void cpu_write8(cpu_t *cpu, uint16_t addr, uint8_t value);
void cpu_step(cpu_t *cpu);

void cpu_unimplemented(cpu_t *cpu, uint8_t opcode);

//CPU opcodes
//bcp is shorthand for BC Pointer. We are loading a memory address into the register, not a value
void nop(); //0x00
void ld_bc_u16(uint16_t value); //0x01
void ld_bcp_a(); //0x02
void inc_bc(); //0x03
void inc_b(); //0x04
void dec_b(); //0x05
void ld_b_u8(uint8_t value); //0x06
void rlca(); //0x07
void ld_u16_sp(uint16_t value); //0x08
void add_hl_bc(); //0x09
void ld_a_bcp(); //0x0a
void dec_bc(); //0x0b
void inc_c(); //0x0c
void dec_c(); //0x0d
void ld_c_u8(uint8_t value); //0x0e
void rrca(); //0x0f
void stop(); //0x10
void ld_de_u16(uint16_t value); //0x11
void ld_dep_a(); //0x12
void inc_de(); //0x13
void inc_d(); //0x14
void dec_d(); //0x15
void ld_d_u8(uint8_t value); //0x16
void rla(); //0x17
void jr_i8(uint8_t value); //0x18

#endif