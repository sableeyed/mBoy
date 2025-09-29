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
void jr_i8(int8_t value); //0x18 -- unconditional jump to relative address
void add_hl_de(); //0x19
void ld_a_dep(); //0x1a
void dec_de(); //0x1b
void inc_e(); //0x1c
void dec_e(); //0x1d
void ld_e_u8(uint8_t value); //0x1e
void rra(); //0x1f
void jr_nz_i8(int8_t value); //0x20 -- conditional jump to relative address if not zero
void ld_hl_u16(uint16_t value); //0x21
void ldi_hlp_a(); //0x22 
void inc_hl(); //0x23
void inc_h(); //0x24
void dec_h(); //0x25
void ld_h_u8(uint8_t value); //0x26
void daa(); //0x27
void jr_z_i8(int8_t value); //0x28 -- conditional jump to relative address if zero
void add_hl_hl(); //0x29
void ldi_a_hlp(); //0x2a
void dec_hl(); //0x2b
void inc_l(); //0x2c
void dec_l(); //0x2d
void ld_l_u8(uint8_t value); //0x2e
void cpl(); //0x2f
void jr_nc_i8(int8_t value); //0x30
void ld_sp_u16(uint16_t value); //0x31
void ld_hlp_a(); //0x32
void inc_sp(); //0x33
void inc_hlp(); //0x34
void dec_hlp(); //0x35
void ld_hlp_u8(uint8_t value); //0x36
void scf(); //0x37
void jr_c_i8(int8_t value); //0x38
void add_hl_sp(); //0x39
void ld_a_hlp(); //0x3a
void dec_sp(); //0x3b
void inc_a(); //0x3c
void dec_a(); //0x3d
void ld_a_u8(uint8_t value); //0x3e
void ccf(); //0x3f
void ld_b(); //0x40
void ld_b_c(); //0x41
void ld_b_d(); //0x42
void ld_b_e(); //0x43
void ld_b_h(); //0x44
void ld_b_l(); //0x45
void ld_b_hlp(); //0x46
void ld_b_a(); //0x47
void ld_c_b(); //0x48
void ld_c(); //0x49
void ld_c_d(); //0x4a
void ld_c_e(); //0x4b
void ld_c_h(); //0x4c
void ld_c_l(); //0x4d
void ld_c_hlp(); //0x4e
void ld_c_a(); //0x4f
void ld_d_b(); //0x50
void ld_d_c(); //0x51
void ld_d(); //0x52
void ld_d_e(); //0x53
void ld_d_h(); //0x54
void ld_d_l(); //0x55
void ld_d_hlp(); //0x56
void ld_d_a(); //0x57
void ld_e_b(); //0x58
void ld_e_c(); //0x59
void ld_e_d(); //0x5a
void ld_e_e(); //0x5b
void ld_e_h(); //0x5c
void ld_e_l(); //0x5d
void ld_e_hlp(); //0x5e
void ld_e_a(); //0x5f
void ld_h_b(); //0x60
void ld_h_c(); //0x61
void ld_h_d(); //0x62
void ld_h_e(); //0x63
void ld_h(); //0x64
void ld_h_l(); //0x65
void ld_h_hlp(); //0x66
void ld_h_a(); //0x67
void ld_l_b(); //0x68
void ld_l_c(); //0x69
void ld_l_d(); //0x6a
void ld_l_e(); //0x6b
void ld_l_h(); //0x6c
void ld_l(); //0x6d
void ld_l_hlp(); //0x6e
void ld_l_a(); //0x6f
void ld_hlp_b(); //0x70
void ld_hlp_c(); //0x71
void ld_hlp_d(); //0x72
void ld_hlp_e(); //0x73
void ld_hlp_h(); //0x74
void ld_hlp_l(); //0x75
void halt(); //0x76
void ld_hlp_a(); //0x77
void ld_a_b(); //0x78
void ld_a_c(); //0x79
void ld_a_d(); //0x7a
void ld_a_e(); //0x7b
void ld_a_h(); //0x7c
void ld_a_l(); //0x7d
void ld_a_hlp(); //0x7e
void ld_a(); //0x7f

#endif