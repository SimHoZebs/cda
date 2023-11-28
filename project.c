#include "spimcore.h"

/* ALU */
/* 10 Points */
// Sim Ho
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
    if (*ALUresult == 1)
        Zero = '1';
    else
        Zero = '1';
}

/* instruction fetch */
/* 10 Points */
// Sim Ho
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
}

/* instruction partition */
/* 10 Points */
// Sim Ho
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
}

/* instruction decode */
/* 15 Points */
//juwel
int instruction_decode(unsigned op, struct_controls *controls)
{

    if (op == 0x00) // R type instruction
    {
        controls->ALUOp = '1';
        controls->ALUSrc = '0';
        controls->Branch = '2';
        controls->Jump = '2';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->MemWrite = '0';
        controls->RegDst = '1';
        controls->RegWrite = '1';
    }
    // I type instuctions
    // else if (op == 0x08) // addi
    // {
    //     controls->ALUOp = '1';
    //     controls->ALUSrc = '1';
    //     controls->Branch = '2';
    //     controls->Jump = '2';
    //     controls->MemRead = '0';
    //     controls->MemtoReg = '0';
    //     controls->MemWrite = '0';
    //     controls->RegDst = '0';
    //     controls->RegWrite = '0';
    // }

    // else if (op == 0x23) // load word
    // {
    //     controls->ALUOp = '0';
    //     controls->ALUSrc = '1';
    //     controls->Branch = '2';
    //     controls->Jump = '2';
    //     controls->MemRead = '0';
    //     controls->MemtoReg = '0';
    //     controls->MemWrite = '0';
    //     controls->RegDst = '0';
    //     controls->RegWrite = '0';
    // }

    // else if (op == 0x2b) // store word
    // {
    //     controls->ALUOp = '0';
    //     controls->ALUSrc = '0';
    //     controls->Branch = '0';
    //     controls->Jump = '0';
    //     controls->MemRead = '0';
    //     controls->MemtoReg = '0';
    //     controls->MemWrite = '0';
    //     controls->RegDst = '0';
    //     controls->RegWrite = '0';
    // }

    // else if (op == 0x0f) // Load Upper Immediate
    // {
    //     controls->ALUOp = '0';
    //     controls->ALUSrc = '0';
    //     controls->Branch = '0';
    //     controls->Jump = '0';
    //     controls->MemRead = '0';
    //     controls->MemtoReg = '0';
    //     controls->MemWrite = '0';
    //     controls->RegDst = '0';
    //     controls->RegWrite = '0';
    // }

    // else if (op == 0x04) // Branch if Equal
    // {
    //     controls->ALUOp = '0';
    //     controls->ALUSrc = '0';
    //     controls->Branch = '0';
    //     controls->Jump = '0';
    //     controls->MemRead = '0';
    //     controls->MemtoReg = '0';
    //     controls->MemWrite = '0';
    //     controls->RegDst = '0';
    //     controls->RegWrite = '0';
    // }

    // else if (op == 0x0b) // set on less tan Immediate Unsigned
    // {
    //     controls->ALUOp = '0';
    //     controls->ALUSrc = '0';
    //     controls->Branch = '0';
    //     controls->Jump = '0';
    //     controls->MemRead = '0';
    //     controls->MemtoReg = '0';
    //     controls->MemWrite = '0';
    //     controls->RegDst = '0';
    //     controls->RegWrite = '0';
    // }

    if (op == 0x08 || op == 0x23 || op == 0x2b || op == 0x0f || op == 0x04 || op == 0x0)
    {
        controls->ALUOp = '1';
        controls->ALUSrc = '1';
        controls->Branch = '2';
        controls->Jump = '2';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->MemWrite = '0';
        controls->RegDst = '0';
        controls->RegWrite = '0';
    }
    // J type instructions

    if (op == 0x02) // jump
    {
        controls->ALUOp = '0';
        controls->ALUSrc = '0';
        controls->Branch = '1';
        controls->Jump = '1';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->MemWrite = '0';
        controls->RegDst = '0';
        controls->RegWrite = '2';
    }
}

/* Read Register */
/* 5 Points */
// Juwel
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2)
{

    // assign data to read_data
    data1 = r1;
    data2 = r2;
}

/* Sign Extend */
/* 10 Points */
//juwel 
void sign_extend(unsigned offset, unsigned *extended_value)
{
}

/* ALU operations */
/* 10 Points */
// Sim Ho
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
}

/* Read / Write Memory */
/* 10 Points */
// juwel
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem)
{
}

/* Write Register */
/* 10 Points */
//juwel
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
}
