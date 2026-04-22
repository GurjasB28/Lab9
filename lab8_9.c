/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
// Please finish the following functions for lab 8.
// Lab 8 will perform the following functions:
//   1. Fetch the code stored in memory
//   2. Decode the code and prepare for the execution of the code.
//   3. Setup the execution function for CPU.

// Lab 9 will perform the following functions:
//   4. Execute the code stored in the memory and print the results.
#include "header.h"
#include "lab8header.h"

extern char *regNameTab[N_REG];
extern int regFile[N_REG];

unsigned int PCRegister = 0;

// ================= CPU =================
void CPU(char *mem){
    unsigned int machineCode = 0;
    unsigned char opcode = 0;

    PCRegister = CODESECTION;

    do{
        printf("\nPC:%08X\n", PCRegister);

        machineCode = CPU_fetchCode(mem, PCRegister);
        printf("Fetched Machine Code: %08X\n", machineCode);

        if (machineCode == 0){
            printf("No more instructions. Stopping CPU.\n");
            break;
        }

        opcode = CPU_Decode(machineCode);
        printf("Decoded Opcode is: %02X\n", opcode);

        PCRegister += 4;

        // ✅ FIX: ACTUALLY EXECUTE
        CPU_Execution(opcode, machineCode, mem);

    } while (1);

    printRegisterFiles();
    printDataMemoryDump(mem);
}
// ================= FETCH =================
unsigned int CPU_fetchCode(char *mem, int codeOffset){
    unsigned int machineCode = 0;

    unsigned char byte0 = (unsigned char)mem[codeOffset];
    unsigned char byte1 = (unsigned char)mem[codeOffset + 1];
    unsigned char byte2 = (unsigned char)mem[codeOffset + 2];
    unsigned char byte3 = (unsigned char)mem[codeOffset + 3];

    machineCode = (unsigned int)byte0 |
                  ((unsigned int)byte1 << 8) |
                  ((unsigned int)byte2 << 16) |
                  ((unsigned int)byte3 << 24);

    return machineCode;
}

// ================= DECODE =================
unsigned char CPU_Decode(unsigned int machineCode){

    unsigned char opcode = 0;
    unsigned char funct = 0;

    unsigned int rs, rt, rd, shamt, immediate, address;

    opcode = (machineCode >> 26) & 0x3F;

    printf("Machine Code: %08X\n", machineCode);
    printf("Opcode: %02X\n", opcode);

    if (opcode == 0){
        rs    = (machineCode >> 21) & 0x1F;
        rt    = (machineCode >> 16) & 0x1F;
        rd    = (machineCode >> 11) & 0x1F;
        shamt = (machineCode >> 6)  & 0x1F;
        funct = machineCode & 0x3F;

        printf("Type: R | rs=%u rt=%u rd=%u shamt=%u funct=%u\n",
               rs, rt, rd, shamt, funct);

        return funct;
    }
    else if (opcode == 2 || opcode == 3){
        address = machineCode & 0x03FFFFFF;

        printf("Type: J | address=%u\n", address);
        return opcode;
    }
    else{
        rs        = (machineCode >> 21) & 0x1F;
        rt        = (machineCode >> 16) & 0x1F;
        immediate = machineCode & 0xFFFF;

        printf("Type: I | rs=%u rt=%u immediate=%u\n",
               rs, rt, immediate);

        return opcode;
    }
}

// ================= EXECUTION =================
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char *mem){

    unsigned char realopcode = 0;
    unsigned char rs = 0;
    unsigned char rt = 0;
    unsigned char rd = 0;
    unsigned int immediate = 0;
    unsigned int address = 0;

    realopcode = (machineCode & 0xFC000000) >> 26;
    rs = (machineCode & 0x03E00000) >> 21;
    rt = (machineCode & 0x001F0000) >> 16;
    rd = (machineCode & 0x0000F800) >> 11;
    immediate = machineCode & 0x0000FFFF;
    address = machineCode & 0x03FFFFFF;

    switch (opcode)
    {
        case 0b101111:
            regFile[rt] = immediate;
            break;

        case 0b100000:
            if (realopcode == 0){
                regFile[rd] = regFile[rs] + regFile[rt];
            } else {
                regFile[rt] = read_byte(mem, regFile[rs] + immediate);
            }
            break;

        case 0b000101:
            if (regFile[rs] >= regFile[rt]){
                PCRegister = PCRegister + (immediate * 4);
            }
            break;

        case 0b100011:
            regFile[rt] = read_dword(mem, regFile[rs] + immediate);
            break;

        case 0b101011:
            write_dword(mem, regFile[rs] + immediate, regFile[rt]);
            break;

        case 0b001000:
            regFile[rt] = regFile[rs] + immediate;
            break;

        case 0b000010:
            PCRegister = address * 4;
            break;

        default:
            printf("Wrong instruction! %02X %08X\n", opcode, machineCode);
            exit(3);
    }
}  // ✅ THIS BRACE WAS MISSING

// ================= REGISTER DUMP =================
void printRegisterFiles(){
    int i;

    printf("\n---- Register File Dump ----\n");
    for (i = 0; i < N_REG; i++){
        printf("%-6s = 0x%08X\n", regNameTab[i], regFile[i]);
    }
}

// ================= MEMORY DUMP =================
void printDataMemoryDump(char *mem){
    printf("\n---- Data Memory Dump ----\n");
    memory_dump(mem, DATASECTION, 256);
}
