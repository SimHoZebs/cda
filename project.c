#include "spimcore.h"

/*
gcc -o spimcore spimcore.c project.c
./spimcore input_file.asc
*/

/* ALU */
/* 10 Points */
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
      *ALUresult = (int)A < (int)B ? 1 : 0;
      break;
    case 0b011:  // set on less than unsigned
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
    default:
      // unhandled ALUControl
      break;
  }

  // set Zero flag if result is zero
  *Zero = *ALUresult ? 0 : 1;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction) {
  // If PC points to an address beyond 64kb or not word-aligned, halt
  if (PC > 65535 | PC & 0x3) {
    return 1;
  }

  // Use PC to get the instruction from Mem and store it on instruction
  *instruction = Mem[PC >> 2];

  return 0;
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,
                           unsigned *r2, unsigned *r3, unsigned *funct,
                           unsigned *offset, unsigned *jsec) {
  //  shifts the bits we want and mask everything else to be 0
  *op = (instruction >> 26) & 0x0000003F;
  *r1 = (instruction >> 21) & 0x0000001F;
  *r2 = (instruction >> 16) & 0x0000001F;
  *r3 = (instruction >> 11) & 0x0000001F;
  *funct = (instruction & 0x0000003F);
  *offset = (instruction & 0x0000FFFF);
  *jsec = (instruction & 0x03FFFFFF);
}

/* instruction decode */
/* 15 Points */
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
  switch (op) {
    case 0x00:
      // R-type instruction
      controls->RegDst = 1;
      controls->ALUOp = 7;  // R-type instruction
      controls->RegWrite = 1;
      break;
    case 0b100011:
      // Load word:
      controls->MemRead = 1;
      controls->MemtoReg = 1;
      controls->ALUSrc = 1;  // instruction is an immediate type
      controls->RegWrite = 1;
      break;
    case 0b101011:
      // Store word:

      controls->RegDst = 2;
      controls->MemtoReg = 2;
      controls->MemWrite = 1;
      controls->ALUSrc = 1;
      break;
    case 0b001010:
      // set less than immediate:

      controls->ALUOp = 2;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      break;
    case 0b001011:
      // set less than immediate unsigned
      // ALUSrc = 1; is an immediate instruction, the ALUop is (0010)
      // ALUOp = 3 (0010) for set less than unsigned
      // RegWrite = 1 because a register is being written
      controls->ALUOp = 3;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      break;
    case 0b000100:
      // Branch Eq:
      // Branch = 1 per instruction
      // ALUSrc = 1 because the second operand for the A      controls->RegDst =
      // 2;
      controls->Branch = 1;
      controls->MemtoReg = 2;
      controls->ALUSrc = 1;
      break;
    case 0b001000:
      // add immediate:
      controls->ALUOp = 0;
      controls->RegWrite = 1;
      controls->ALUSrc = 1;
      break;
    case 0b000010:
      // Ju      controls->RegDst = 2;
      controls->Jump = 1;
      controls->Branch = 2;
      controls->MemRead = 1;
      controls->MemtoReg = 2;
      controls->ALUSrc = 2;
      break;
    case 0b001111:
      // Load Upper Immedi      controls->RegDst = 1;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      controls->ALUOp = 6;
      break;
    default:
      // unhandled upc      return 1;
      break;
  }

  return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1,
                   unsigned *data2) {
  // assign  register values to data by retreaving it from the Reg ptr Array
  *data1 = Reg[r1];
  *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
// Assign the sign-extended value of offset to extended value.

void sign_extend(unsigned offset, unsigned *extended_value) {
  // used to normalise the 16 bit immedate valuebefore passing the value to the
  // ALU (ALU takes 32 bits) find the MSB, if it's zero extend the value to 16
  // zeros, if it's one extend the value to 16 ones
  int res = 1 << 15;

  // check if MSB is 1
  if (res & offset) {
    // is negative value; add ones to the left per two's complement
    *extended_value = offset | 0xFFFF0000;
  } else {
    // positive values are left alone
    *extended_value = offset;
  }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value,
                   unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult,
                   char *Zero) {
  *ALUresult = 0;

  // R-type
  if (ALUSrc == 0) {
    int alu;
    switch (funct) {
      case 0b100100:
        // AND
        alu = 4;
        break;
      case 0b100110:
        // XOR
        alu = 5;
        break;
      case 0b100000:
        // add
        alu = 0;
        break;
      case 0b100010:
        // subtract
        alu = 1;
        break;
      case 0b101010:
        // set less than
        alu = 2;
        break;
      case 0b101011:
        // set less than unsigned
        alu = 3;
        break;
      default:
        // not handled
        return 1;
        break;
    }
    ALU(data1, data2, alu, ALUresult, Zero);
  }
  // I-type
  else if (ALUSrc == 1) {  // add immediate
    // load upper immediate
    // set less than immediate
    // set less than immediate unsigned
    if (ALUOp == 0) {
    }
    ALU(data1, extended_value, ALUOp, ALUresult, Zero);
  }

  return 0;
}

/* Read / Write Memory */
/* 10 Points */

int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead,
              unsigned *memdata, unsigned *Mem) {
  // return 1 on halt condition

  if ((MemWrite == 1 || MemRead == 1) && ALUresult % 4 != 0) {
    return 1;
  }

  if (MemRead) *memdata = Mem[ALUresult >> 2];
  if (MemWrite) Mem[ALUresult >> 2] = data2;

  return 0;
}
/* Write Register */

/* 10 Points */
// juwel
void write_register(unsigned r2, unsigned r3, unsigned memdata,
                    unsigned ALUresult, char RegWrite, char RegDst,
                    char MemtoReg, unsigned *Reg) {
  /*
  if the  control.RegWrite is true writing to the register is enabled by
  accessing the Register memory through Reg ptr.
  */
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
  *PC = *PC + 4;

  if (Jump == 1) {
    /*
    if jump is 1, the PC equals the jump target shifted 2-bits left ORed
    withthe upper four bits of the PC to make 32-bits */
    *PC = (jsec << 2) | (*PC | 0xf0000000);
  }
  /*If branch is 1, the PC will be equal to the sign extended value sifted left
    two bits  */
  else if (Branch == 1 && Zero) {
    *PC += (extended_value << 2);
  }
}
