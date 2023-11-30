#include "spimcore.h"

/* ALU */
/* 10 Points */
// Sim Ho
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult,
         char *Zero) {
  switch (ALUControl) {
    case 0b000:  // Z = A + B
      *ALUresult = A + B;
      break;
    case 0b001:  // Z = A - B
      *ALUresult = A - B;
      break;
    case 0b010:  // set on less than
      *ALUresult = A < B ? 1 : 0;
      break;
    case 0b100:  // Z = A AND B
      *ALUresult = A & B;
      break;
    case 0b101:  // Z= A XOR B
      *ALUresult = A ^ B;
      break;
    case 0b110:  // Z = B << 16
      *ALUresult = B << 16;
      break;
    case 0b111:  // Z = NOT A
      *ALUresult = ~A;
      break;
  }

  // set Zero flag if result is zero
  *Zero = *ALUresult ? 0 : 1;

  // if (*ALUresult == 1)
  //     Zero = '1';
  // else
  //     Zero = '1';
}

/* instruction fetch */
/* 10 Points */
// Sim Ho
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction) {
  // Lorena
  // halt conditions
  if (Mem[PC] % 4 != 0)
    return 1;  // Jumping to an address that is not word-aligned
  if (Mem[PC] > 0x13FFF)
    return 1;  // Accessing data or jumping to an address that is beyond the
               // memory

  // Get the instruction from Mem and store it on the instruction variable
  *instruction = Mem[PC >> 2];

  return 0;
}

/* instruction partition */
/* 10 Points */
// Sim Ho
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,
                           unsigned *r2, unsigned *r3, unsigned *funct,
                           unsigned *offset, unsigned *jsec) {
  // Sim Ho
  //  extract op code
  *op = (instruction >> 26) & 0x0000003F;
  *r1 = (instruction >> 21) & 0x03E00000;
  *r2 = (instruction >> 16) & 0x001F0000;
  *r3 = (instruction >> 11) & 0x0000F800;
  *funct = (instruction & 0x0000003F);
  *offset = (instruction & 0x0000FFFF);
  *jsec = (instruction & 0x03FFFFFF);

  // Lorina
  //  get the first six bits for the opcode
  *op = (instruction >> 26) & 0x0000003F;

  // get the five bits for register 1
  *r1 = (instruction >> 21) & 0x0000001F;

  // get the five bits for register 2
  *r2 = (instruction >> 16) & 0x0000001F;

  // get the five bits for register 3
  *r3 = (instruction >> 11) & 0x0000001F;

  // get the first six-bits for the function
  *funct = instruction & 0x0000003F;

  // get the first 16-bits for the offset)
  *offset = instruction & 0x0000FFFF;

  // get the first 26-bits for jump to target address
  *jsec = instruction & 0x03FFFFFF;
}

/* instruction decode */
/* 15 Points */
// juwel
int instruction_decode(unsigned op, struct_controls *controls) {
  // default values
  controls->RegDst = 0;  // only R-types have RegDst = 1
  controls->Jump = 0;
  controls->Branch = 0;
  controls->MemRead = 0;
  controls->MemtoReg = 0;
  controls->ALUOp = 0;
  controls->MemWrite = 0;
  controls->ALUSrc = 0;  // only R-types have ALUSrc = 0
  controls->RegWrite = 0;

  if (op == 0x00)  // R type instruction
  {
    controls->ALUOp = 1;
    controls->ALUSrc = 0;
    controls->Branch = 2;
    controls->Jump = 2;
    controls->RegDst = 1;
    controls->RegWrite = 1;
  }

  if (op == 0x08 || op == 0x23 || op == 0x2b || op == 0x0f || op == 0x04 ||
      op == 0x0) {
    controls->ALUOp = 1;
    controls->ALUSrc = 1;
    controls->Branch = 2;
    controls->Jump = 2;
  }
  // J type instructions

  if (op == 0x02)  // jump
  {
    controls->Branch = 1;
    controls->Jump = 1;
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

  // else if (op == 0x04) // Branch if
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
}

/* Read Register */
/* 5 Points */
// Juwel
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1,
                   unsigned *data2) {
  // assign data to read_data
  data1 = r1;
  data2 = r2;
}

/* Sign Extend */
/* 10 Points */
// Assign the sign-extended value of offset to extended value.

// juwel
void sign_extend(unsigned offset, unsigned *extended_value) {}

/* ALU operations */
/* 10 Points */
// Sim Ho
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value,
                   unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult,
                   char *Zero) {
  // R-type
  if (ALUSrc == 0) {
    //  branch on equal
    //  set less than
    //  set less than unsigned
    //  jump
    switch (ALUOp) {
      case 0b000:  // add
        ALU(data1, data2, 0b000, ALUresult, Zero);
        break;
      case 0b001:  // subtract
        ALU(data1, data2, 0b001, ALUresult, Zero);
        break;
      case 0b010:  // set on less than
        ALU((signed)data1, (signed)data2, 0b010, ALUresult, Zero);
        break;
      case 0b011:  // set on less than unsigned
        ALU(data1, data2, 0b010, ALUresult, Zero);
        break;
      case 0b100:  // AND
        ALU(data1, data2, 0b100, ALUresult, Zero);
        break;
      case 0b101:  // XOR
        ALU(data1, data2, 0b101, ALUresult, Zero);
        break;
      case 0b110:  // Z = B << 16
        ALU(data1, data2, 0b110, ALUresult, Zero);
        break;
      default:  // rest of R-type functions
        ALU(data1, data2, funct, ALUresult, Zero);
        break;
    }
  }
  // I-type
  else {
    // add immediate
    // load upper immediate
    // set less than immediate
    // set less than immediate unsigned
  }
}

/* Read / Write Memory */
/* 10 Points */
// juwel
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead,
              unsigned *memdata, unsigned *Mem) {
  if (MemRead) *memdata = Mem[ALUresult >> 2];

  if (MemWrite) Mem[ALUresult >> 2] = data2;
  // return 1 on halt condition

  return 0;
}

/* Write Register */
/* 10 Points */
// juwel
void write_register(unsigned r2, unsigned r3, unsigned memdata,
                    unsigned ALUresult, char RegWrite, char RegDst,
                    char MemtoReg, unsigned *Reg) {
  if (RegWrite) {
    if (MemtoReg) {
      if (RegDst)
        Reg[r3] = memdata;
      else
        Reg[r2] = memdata;
    } else {
      if (RegDst)
        Reg[r3] = ALUresult;
      else
        Reg[r2] = ALUresult;
    }
  }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump,
               char Zero, unsigned *PC) {
  PC += 4;
  // int instuctionSet = (jsec << 2) & 0b11111111111111111111111100;\

  int jumpAddress = instuctionSet | (int)PC;

  // combine PC+4 with instrcoin
  //  instructionSet =

  //

  // shift and extend operation
  int shiftLeft = extended_value << 2;
  PC = PC + shiftLeft;
}
