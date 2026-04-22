/*
 ============================================================================
 Name        : Lab6.c
 Author      : Austin Tian
 Revised by  :
 Version     :
 Copyright   : Copyright 2023
 Description : Lab 6 in C, ANSI-C Style
 ============================================================================
 */

#include "header.h"

int n_Label = 0;
MIPS_Instruction Instruction_storage[MAX_SIZE];
int regFile[N_REG];
char Data_storage[MAX_SIZE];
unsigned int totalDataByte;
labelType labelTab[MAX_LABEL];

// Menu for the testing.
char *menu =    "\n" \
                " ***********Please select the following options**********************\n" \
                " *    This is the memory operation menu (Lab 6)                     *\n" \
                " ********************************************************************\n" \
                " *    1. Write a double-word (32-bit) to the memory                 *\n"  \
                " ********************************************************************\n" \
                " *    2. Read a byte (8-bit) data from the memory                   *\n" \
                " *    3. Read a double-word (32-bit) data from the memory           *\n" \
                " ********************************************************************\n" \
                " *    4. Generate a memory dump from any memory location            *\n" \
                " ********************************************************************\n" \
                " *    e. To Exit, Type 'e'  or 'E'                                  *\n" \
                " ********************************************************************\n";

//---------------------------------------------------------------
// Generate a random number between 0x00 and 0xFF.
unsigned char rand_generator()
{
    return rand()%255;
}
//---------------------------------------------------------------
void free_memory(char *base_address)
{
    free(base_address);
    return;
}
//---------------------------------------------------------------
char *init_memory()
{
    char *mem = malloc(MEM_SIZE);
    if (mem == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    for (unsigned int i = 0; i < MEM_SIZE; i++)
    {
        mem[i] = rand_generator();
    }

    return mem;
}

//---------------------------------------------------------------
void write_byte(const char *base_address, const int offset, const unsigned char byte_data){
    if (offset < 0 || offset >= MEM_SIZE)
    {
        printf("Invalid offset.\n");
        return;
    }

    char *mem = (char *)base_address;
    mem[offset] = byte_data;
}

//---------------------------------------------------------------
void write_dword(const char *base_address, const int offset, const unsigned int dword_data){
    // Step 2: write a double-word to address: "base_address + offset".
    if (offset < 0 || offset + 3 >= MEM_SIZE)
    {
        printf("Invalid offset.\n");
        return;
    }

    char *mem = (char *)base_address;
    mem[offset]     = (dword_data & 0xFF);
    mem[offset + 1] = (dword_data >> 8) & 0xFF;
    mem[offset + 2] = (dword_data >> 16) & 0xFF;
    mem[offset + 3] = (dword_data >> 24) & 0xFF;

    printf("Double-word 0x%08X written at offset 0x%X\n", dword_data, offset);
}
//---------------------------------------------------------------
unsigned char read_byte(const char *base_address, const int offset){
    // Step 3: return and print the byte from address: "base_address + offset".

    if (offset < 0 || offset >= MEM_SIZE)
    {
        printf("Invalid offset.\n");
        return 0;
    }

    unsigned char value = (unsigned char)base_address[offset];

    printf("Byte at offset 0x%X: 0x%02X\n", offset, value);

    return value;
}
//---------------------------------------------------------------
unsigned int read_dword(const char *base_address, const int offset){
    // Step 4: return and print the double-word from address: "base_address + offset".

    if (offset < 0 || offset + 3 >= MEM_SIZE)
    {
        printf("Invalid offset.\n");
        return 0;
    }

    unsigned int value = 0;

    value |= (unsigned char)base_address[offset];
    value |= ((unsigned char)base_address[offset + 1]) << 8;
    value |= ((unsigned char)base_address[offset + 2]) << 16;
    value |= ((unsigned char)base_address[offset + 3]) << 24;

    printf("Double-word at offset 0x%X: 0x%08X\n", offset, value);

    return value;
}
//---------------------------------------------------------------
void memory_dump(const char *base_address, const int offset, unsigned int dumpsize){
    // Step 5: Generate a memory dump display starting from address "base_address + offset".

    if (dumpsize < MIN_DUMP_SIZE || dumpsize > MEM_SIZE)
        dumpsize = MIN_DUMP_SIZE;

    if (offset < 0 || offset >= MEM_SIZE)
    {
        printf("Invalid offset.\n");
        return;
    }

    for (unsigned int i = 0; i < dumpsize; i += DUMP_LINE)
    {
        printf("%08X  ", offset + i);

        for (int j = 0; j < DUMP_LINE; j++)
        {
            if (offset + i + j < MEM_SIZE && i + j < dumpsize)
                printf("%02X ", (unsigned char)base_address[offset + i + j]);
            else
                printf("   ");
        }

        printf(" ");

        for (int j = 0; j < DUMP_LINE; j++)
        {
            if (offset + i + j < MEM_SIZE && i + j < dumpsize)
            {
                unsigned char c = base_address[offset + i + j];

                if (c >= 0x20 && c <= 0x7E)
                    printf("%c", c);
                else
                    printf(".");
            }
        }

        printf("\n");
    }

    return;
}

//---------------------------------------------------------------
void setup_memory()
{
    char *mem = init_memory();
    char options =0;
    unsigned int offset, dumpsize;
    char tempchar;
    unsigned int dword_data;

    do{
        if (options != 0x0a)
        {
            puts(menu);
            printf ("\nThe base address of your memory is: %I64Xh (HEX)\n", (long long unsigned int)(mem));
            puts("Please make a selection:");
        }

        options = getchar();

        switch (options)
        {
            case '1':
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", &offset);
                puts("Please input your DOUBLE WORD data to be written (in HEX):");
                scanf("%x", &dword_data);
                write_dword (mem, offset, dword_data);
                continue;

            case '2':
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", &offset);
                read_byte(mem, offset);
                continue;

            case '3':
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", &offset);
                read_dword(mem, offset);
                continue;

            case '4':
                puts("Please input your memory's offset address (in HEX, should be a multiple of 0x10h):");
                scanf("%x", &offset);
                puts("Please input the size of the memory to be dumped (a number between 256 and 1024 ):");
                scanf("%d", &dumpsize);
                memory_dump(mem, offset, dumpsize);
                continue;

            case 'e':
            case 'E':
                puts("Code finished, press any key to exit");
                free_memory(mem);
                while ((tempchar = getchar()) != '\n' && tempchar != EOF);
                tempchar = getchar();
                return;

            default:
                continue;
        }


    }while (1);

    return;
}
