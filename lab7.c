/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "header.h"

// This is the function to fill in the data section.
void setupDataMemory(        char* base_address,
                             unsigned int offset,
                             char *datasection,
                             unsigned int numberOfBytes){
    // Finish this function to fill in datasection starting from the offset in the memory.
    for(unsigned int i = 0; i < numberOfBytes; i++){
        write_byte(base_address, offset + i, datasection[i]);
    }
    return;

}

// This is the function to build I type instruction based on Instruction_storage
unsigned int buildIInstruction(unsigned char opcode,
                               unsigned char rs,
                               unsigned char rt,
                               int immediate){
    unsigned int machineCode = 0;
    unsigned int mask = 0;

    machineCode = immediate & 0x0000FFFF;

    mask = ((unsigned int)(rs & 0xFF)) << 21;
    machineCode |= mask;

    mask = ((unsigned int)(rt & 0xFF)) << 16;
    machineCode |= mask;

    mask = ((unsigned int)(opcode & 0xFF)) << 26;
    machineCode |= mask;

    return machineCode;
}

// This is the function to build J type instruction based on Instruction_storage
unsigned int buildJInstruction(unsigned char opcode,
                               int immediate){
     unsigned int machineCode = 0;
     unsigned int mask = 0;

     mask = (immediate & 0x03FFFFFF);
     machineCode = mask;

     mask = ((unsigned int)(opcode & 0xFF)) << 26;
     machineCode |= mask;

    return machineCode;

}

// This is the function to build R type instruction based on Instruction_storage
unsigned int buildRInstruction(unsigned char opcode,
                               unsigned char rs,
                               unsigned char rt,
                               unsigned char rd,
                               unsigned char shamt,
                               unsigned char function){
     unsigned int machineCode = 0;
     unsigned int mask = 0;

     mask = function & 0x3F;
     machineCode = mask;

     mask = ((unsigned int)(shamt & 0x1F)) << 6;
     machineCode |= mask;

     mask = ((unsigned int)(rd & 0x1F)) << 11;
     machineCode |= mask;

     mask = ((unsigned int)(rt & 0x1F)) << 16;
     machineCode |= mask;

     mask = ((unsigned int)(rs & 0x1F)) << 21;
     machineCode |= mask;

     mask = ((unsigned int)(opcode & 0x3F)) << 26;
     machineCode |= mask;

    return machineCode;

}

// This is the function to store the instructions.
void setupInstructionMemory( char* base_memory_address,
                                     int codeOffset,
                                     MIPS_Instruction *instructionStorage){
    int i = 0;
    unsigned int totalinstruction = 0;
    unsigned char opcode = 0;
    unsigned int machineCode = 0;

    do {
        // if la instruction.
        if (strcmp(instructionStorage[i].instruction, "la") == 0 ){

        // First, Answer this: what type of instruction is this? J, R or I instruction?
        // Answer: This is an I type instruction

        // Secondly, what kind of machine language data format should it be?
        // Answer: It follows the I-format: opcode, rs, rt, and immediate.
        // needs: opcode, rs, rt, immediate

        // Thirdly, what is the opcode of this instruction?
        // Answer: The opcode is 0b101111.

            opcode = 0b101111;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);

            write_dword(base_memory_address, codeOffset+i*4, machineCode);
        }

        else if (strcmp(instructionStorage[i].instruction, "lb") == 0 ){

        // First, Answer this: what type of instruction is this? J, R or I instruction?
        // Answer: This is also an I type instruction.

        // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
        // Answer: It uses the I-type format
        // needs: opcode, rs, rt, immediate

        // Thirdly, what is the opcode of this instruction?
        // Answer: The opcode for lb is 0b100000.

            opcode = 0b100000;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);

            write_dword(base_memory_address, codeOffset+i*4, machineCode);
        }

        else if (strcmp(instructionStorage[i].instruction, "bge") == 0 ){

        // First, Answer this: what type of instruction is this? J, R or I instruction?
        // Answer: This is an I type instruction.

        // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
        // Answer: It follows the I format using opcode, rs, rt and immediate offset.
        // needs: opcode, rs, rt, immediate

        // Thirdly, what is the opcode of this instruction?
        // Answer: The opcode here is 0b000001.

            opcode = 0b000101;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);

            write_dword(base_memory_address, codeOffset+i*4, machineCode);
        }

        else if (strcmp(instructionStorage[i].instruction, "lw") == 0 ){

        // First, Answer this: what type of instruction is this? J, R or I instruction?
        // Answer: lw is an I type instruction.

        // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
        // Answer: It uses opcode, rs (base), rt (destination), and immediate offset.
        // needs: opcode, rs, rt, immediate

        // Thirdly, what is the opcode of this instruction?
        // Answer: The opcode for lw is 0b100011.

            opcode = 0b100011;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);

            write_dword(base_memory_address, codeOffset+i*4, machineCode);
        }

        else if (strcmp(instructionStorage[i].instruction, "sw") == 0 ){

        // First, Answer this: what type of instruction is this? J, R or I instruction?
        // Answer: sw is also an I type instruction.

        // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
        // Answer: Same format as lw: opcode, rs, rt, and immediate.
        // needs: opcode, rs, rt, immediate

        // Thirdly, what is the opcode of this instruction?
        // Answer: The opcode for sw is 0b101011.

            opcode = 0b101011;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);

            write_dword(base_memory_address, codeOffset+i*4, machineCode);
        }

		else if (strcmp(instructionStorage[i].instruction, "add") == 0 ){

        // First, Answer this: what type of instruction is this? J, R or I instruction?
        // Answer: add is an R type instruction.

        // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
        // Answer: It uses rs, rt, rd, shamt, and funct fields.
        // needs: rs, rt, rd, shamt, funct

        // Thirdly, what is the opcode of this instruction?
        // Answer: The opcode is 0 (R-type).

            opcode = 0;
            machineCode = buildRInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].rd,
                                            0,
                                            32);

            write_dword(base_memory_address, codeOffset+i*4, machineCode);
        }

        else if (strcmp(instructionStorage[i].instruction, "addi") == 0 ){

        // First, Answer this: what type of instruction is this? J, R or I instruction?
        // Answer: addi is an I type instruction.

        // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
        // Answer: It uses opcode, rs, rt, and immediate.
        // needs: opcode, rs, rt, immediate

        // Thirdly, what is the opcode of this instruction?
        // Answer: The opcode for addi is 0b001000.

            opcode = 0b001000;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);

            write_dword(base_memory_address, codeOffset+i*4, machineCode);
        }

        else if (strcmp(instructionStorage[i].instruction, "j") == 0 ){

        // First, Answer this: what type of instruction is this? J, R or I instruction?
        // Answer: j is a j type instruction.

        // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
        // Answer: It uses opcode and a 26-bit address.
        // needs: opcode, address

        // Thirdly, what is the opcode of this instruction?
        // Answer: The opcode is 0b000101.

            opcode = 0b000101;
            machineCode = buildJInstruction(opcode,
                                            instructionStorage[i].address);
            write_dword(base_memory_address, codeOffset+i*4, machineCode);
        }

        else if (strcmp(instructionStorage[i].instruction, "syscall") == 0 ||
                 (strcmp(instructionStorage[i].instruction, "END") == 0 ))
        {
            break;
        }

        i++;
    }while (1);
    totalinstruction = i;
}
// load the code into the memory starts from offset.
void loadCodeToMem(char *mem){
    unsigned int dataSection = DATASECTION;
    unsigned int instructionSection = CODESECTION;
    setupDataMemory(mem, dataSection, Data_storage, totalDataByte);
    setupInstructionMemory(mem, instructionSection, Instruction_storage);
    puts("\n---- Data Section ----\n");
    memory_dump(mem, DATASECTION, 256);
    puts("\n---- Code Section ----\n");
    memory_dump(mem, CODESECTION, 256);
}
