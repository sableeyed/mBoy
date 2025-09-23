#include "cpu.h"
#include "registers.h"
#include <string.h>
#include <stdio.h>

//Refactor incoming - memory to be separated

void cpu_init(cpu_t *cpu, cartridge_t *cart) {
    if(!cpu || !cart) {
        fprintf(stderr, "CPU init failed\n");
        return;
    }

    memset(cpu->memory, 0x00, sizeof(cpu->memory));

    size_t copy_size = (cart->size > 0x8000) ? 0x8000 : cart->size;
    memcpy(cpu->memory, cart->rom, copy_size);
}

uint8_t cpu_read8(cpu_t *cpu, uint16_t addr) {
    return cpu->memory[addr];
}

void cpu_write8(cpu_t *cpu, uint16_t addr, uint8_t value) {
    cpu->memory[addr] = value;
}

void cpu_step(cpu_t *cpu) {
    if (!cpu) { return; }

    static uint16_t pc = 0x0000;
    uint8_t opcode = cpu_read8(cpu, pc);

    cpu_unimplemented(cpu, opcode);
    pc++;
}

void cpu_unimplemented(cpu_t *cpu, uint8_t opcode) {
    char buf[64];
    snprintf(buf, sizeof(buf), "Opcode 0x%02X not implemented!", opcode);

    if (cpu && cpu->hwnd) {
        int result = MessageBox(cpu->hwnd, buf, "MasochistBoy CPU", MB_OKCANCEL | MB_ICONERROR);
        if (result == IDCANCEL) { exit(1); }
    } 
    else {
        fprintf(stderr, "%s\n", buf);
    }
}