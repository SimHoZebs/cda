#include "spimcore.h"

/*
gcc -o spimcore spimcore.c project.c
./spimcore input_file.asc
*/

/* ALU */
/* 10 Points */
// Sim Ho
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult,
         char *Zero) {
  printf("---ALU---\n");
  printf("A: %d, B: %d, ALUControl: %d\n", A, B, ALUControl);
  switch (ALUControl) {
    case 0b000:  // Z = A + B
      printf("add\n");
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
      printf("halt\n");
      break;
  }

  printf("ALUresult: %d\n", *ALUresult);
  // set Zero flag if result is zero
  *Zero = *ALUresult ? 0 : 1;
}

/* instruction fetch */
/* 10 Points */
// Sim Ho
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction) {
  printf("--- instruction fetch ---\n");
  // Lorena

  // If PC points to an address beyond 64kb or not word-aligned, halt
  if (PC > 65535 | PC & 0x3) {
    printf("halt\n");
    return 1;
  }

  // Use PC to get the instruction from Mem and store it on instruction
  *instruction = Mem[PC >> 2];

  return 0;
}

/* instruction partition */
/* 10 Points */
// Sim Ho
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,
                           unsigned *r2, unsigned *r3, unsigned *funct,
                           unsigned *offset, unsigned *jsec) {
  printf("--- instruction partition ---\n");
  // Sim Ho

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

  printf("---instruction decode---\n");
  printf("op %u\n", op);

  switch (op) {
    case 0x00:
      printf("received R type\n");
      // R-type instruction
      controls->RegDst = 1;
      controls->ALUOp = 7;  // R-type instruction
      controls->RegWrite = 1;
      break;
    case 0b100011:
      printf("load word\n");
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
      // ALUSrc = 1 because the second operand for the ALU
      printf("Branch is equal\n");
      controls->RegDst = 2;
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
      // Jump:
      printf("jump\n");
      controls->RegDst = 2;
      controls->Jump = 1;
      controls->Branch = 2;
      controls->MemRead = 1;
      controls->MemtoReg = 2;
      controls->ALUSrc = 2;
      break;
    case 0b001111:
      // Load Upper Immediate
      printf("load upper Immediate\n");
      controls->RegDst = 1;
      controls->ALUSrc = 1;
      controls->RegWrite = 1;
      controls->ALUOp = 6;
      break;
    default:
      // unhandled upcode
      printf("halt\n");
      return 1;
      break;
  }

  return 0;
}

/* Read Register */
/* 5 Points */
// Juwel
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1,
                   unsigned *data2) {
  printf("--- read register ---\n");
  // assign data to read_data
  *data1 = Reg[r1];
  *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
// Assign the sign-extended value of offset to extended value.

// juwel
void sign_extend(unsigned offset, unsigned *extended_value) {
  printf("--- sign extend ---\n");
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
// Sim Ho
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value,
                   unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult,
                   char *Zero) {
  printf("--- ALU_operations ---\n");
  *ALUresult = 0;

  // R-type
  if (ALUSrc == 0) {
    printf("is R type\n");
    int alu;
    switch (funct) {
      case 0b100100:
        printf("and\n");
        // AND
        alu = 4;
        break;
      case 0b100110:
        printf("xor\n");
        // XOR
        alu = 5;
        break;
      case 0b100000:
        printf("add\n");
        // add
        alu = 0;
        break;
      case 0b100010:
        printf("subtract\n");
        // subtract
        alu = 1;
        break;
      case 0b101010:
        printf("set less than\n");
        // set less than
        alu = 2;
        break;
      case 0b101011:
        // set less than unsigned
        printf("set less than unsigned\n");
        alu = 3;
        break;
      default:
        printf("unhandled funct\n");
        // not handled
        return 1;
        break;
    }
    ALU(data1, data2, alu, ALUresult, Zero);
  }
  // I-type
  else if (ALUSrc == 1) {
    printf("is i type\n");
    // add immediate
    // load upper immediate
    // set less than immediate
    // set less than immediate unsigned
    if (ALUOp == 0) {
      printf("Add immediate in ALU_operations\n");
    }
    ALU(data1, extended_value, ALUOp, ALUresult, Zero);
  }

  return 0;
}

/* Read / Write Memory */
/* 10 Points */
// juwel
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead,
              unsigned *memdata, unsigned *Mem) {
  printf("--- rw memory ---\n");
  // return 1 on halt condition

  printf(
      "MemWrite: %d, MemRead: %d, ALUResult: %d, data2: %d, memdata: %d, Mem: "
      "%d\n",
      MemWrite, MemRead, ALUresult, data2, *memdata, *Mem);

  if ((MemWrite == 1 || MemRead == 1) && ALUresult % 4 != 0) {
    printf("halt\n");
    return 1;
  }

  printf("MemRead issue at Mem[ALUresult]");
  if (MemRead) *memdata = Mem[ALUresult >> 2];
  printf("MemWrite issue");
  if (MemWrite) Mem[ALUresult >> 2] = data2;

  return 0;
}
/* Write Register */

/* 10 Points */
// juwel
void write_register(unsigned r2, unsigned r3, unsigned memdata,
                    unsigned ALUresult, char RegWrite, char RegDst,
                    char MemtoReg, unsigned *Reg) {
  printf("--- write register ---\n");
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
  printf("--- PC update ---\n");
  *PC = *PC + 4;

  if (Jump == 1) {
    /*if jump is 1, the PC equals the jump target shifted 2-bits left ORed
  with
     * the upper four bits of the PC to make 32-bits*/
    *PC = (jsec << 2) | (*PC | 0xf0000000);  // dont do &
  }
  /*If branch is 1, the PC will be equal to the sign extended value shifted left
     two bits  */
  else if (Branch == 1 && Zero) {
    *PC += (extended_value << 2);
  }
}

/*
else if (Branch == 1 && Zero) {
  *PC += (extended_value << 2);
}

*PC += 4;                                       // first pc add operation
int instuctionSet = (jsec << 2);                // shift instuction
int tempPCShift = *PC << 28;                    // allign pc as the 28-31 bits
int jumpAddress = instuctionSet | tempPCShift;  // combine into jump address

int brachInt = Branch - '0';
int zeroInt = Zero - '0';
int jumpInt = Jump - '0';

// Simulate an AND gate
int mux_And_Op = brachInt && zeroInt;
int mux_output;
if (mux_And_Op) {  // mulitplex 1

  // Second ALU add operation
  int extendShifted = extended_value << 2;
  mux_output = extendShifted + *PC;
} else
  mux_output = *PC;

if (jumpInt)  // mulitplex 2
  *PC = jumpAddress;
else
  *PC = mux_output;

*/
