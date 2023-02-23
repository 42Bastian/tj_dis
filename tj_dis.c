#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>

enum opcodesTom_e {
  /*  0 */ opcode_ADD = 0,
  /*  1 */ opcode_ADDC,
  /*  2 */ opcode_ADDQ,
  /*  3 */ opcode_ADDQT,
  /*  4 */ opcode_SUB,
  /*  5 */ opcode_SUBC,
  /*  6 */ opcode_SUBQ,
  /*  7 */ opcode_SUBQT,
  /*  8 */ opcode_NEG,
  /*  9 */ opcode_AND,
  /* 10 */ opcode_OR,
  /* 11 */ opcode_XOR,
  /* 12 */ opcode_NOT,
  /* 13 */ opcode_BTST,
  /* 14 */ opcode_BSET,
  /* 15 */ opcode_BCLR,
  /* 16 */ opcode_MULT,
  /* 17 */ opcode_IMULT,
  /* 18 */ opcode_IMULTN,
  /* 19 */ opcode_RESMAC,
  /* 20 */ opcode_IMACN,
  /* 21 */ opcode_DIV,
  /* 22 */ opcode_ABS,
  /* 23 */ opcode_SH,
  /* 24 */ opcode_SHLQ,
  /* 25 */ opcode_SHRQ,
  /* 26 */ opcode_SHA,
  /* 27 */ opcode_SHARQ,
  /* 28 */ opcode_ROR,
  /* 29 */ opcode_RORQ,
  /* 30 */ opcode_CMP,
  /* 31 */ opcode_CMPQ,
  /* 32 */ opcode_SAT8,
  /* 33 */ opcode_SAT16,
  /* 34 */ opcode_MOVE,
  /* 35 */ opcode_MOVEQ,
  /* 36 */ opcode_MOVETA,
  /* 37 */ opcode_MOVEFA,
  /* 38 */ opcode_MOVEI,
  /* 39 */ opcode_LOADB,
  /* 40 */ opcode_LOADW,
  /* 41 */ opcode_LOAD,
  /* 42 */ opcode_LOADP,
  /* 43 */ opcode_LOAD_R14,
  /* 44 */ opcode_LOAD_R15,
  /* 45 */ opcode_STOREB,
  /* 46 */ opcode_STOREW,
  /* 47 */ opcode_STORE,
  /* 48 */ opcode_STOREP,
  /* 49 */ opcode_STORE_R14,
  /* 50 */ opcode_STORE_R15,
  /* 51 */ opcode_MOVE_PC,
  /* 52 */ opcode_JUMP,
  /* 53 */ opcode_JR,
  /* 54 */ opcode_MMULT,
  /* 55 */ opcode_MTOI,
  /* 56 */ opcode_NORMI,
  /* 57 */ opcode_NOP,
  /* 58 */ opcode_LOAD_R14_RN,
  /* 59 */ opcode_LOAD_R15_RN,
  /* 60 */ opcode_STORE_R14_RN = 60,
  /* 61 */ opcode_STORE_R15_RN,
  /* 62 */ opcode_SAT24,
  /* 63 */ opcode_PACK = 63,
  opcode_UNPACK = 63
};

const char * opcodesTomTxt[64] = {
  /*  0 */ "add",
  /*  1 */ "addc",
  /*  2 */ "addq",
  /*  3 */ "addqt",
  /*  4 */ "sub",
  /*  5 */ "subc",
  /*  6 */ "subq",
  /*  7 */ "subqt",

  /*  8 */ "neg",
  /*  9 */ "and",
  /* 10 */ "or",
  /* 11 */ "xor",
  /* 12 */ "not",
  /* 13 */ "btst",
  /* 14 */ "bset",
  /* 15 */ "bclr",

  /* 16 */ "mult",
  /* 17 */ "imult",
  /* 18 */ "imultn",
  /* 19 */ "resmac",
  /* 20 */ "imacn",
  /* 21 */ "div",
  /* 22 */ "abs",
  /* 23 */ "sh",

  /* 24 */ "shlq",
  /* 25 */ "shrq",
  /* 26 */ "sha",
  /* 27 */ "sharq",
  /* 28 */ "ror",
  /* 29 */ "rorq",
  /* 30 */ "cmp",
  /* 31 */ "cmpq",

  /* 32 */ "sat8",
  /* 33 */ "sat16",
  /* 34 */ "move",
  /* 35 */ "moveq",
  /* 36 */ "moveta",
  /* 37 */ "movefa",
  /* 38 */ "movei",
  /* 39 */ "loadb",

  /* 40 */ "loadw",
  /* 41 */ "load",
  /* 42 */ "loadp",
  /* 43 */ "load",
  /* 44 */ "load",
  /* 45 */ "storeb",
  /* 46 */ "storew",
  /* 47 */ "store",

  /* 48 */ "storep",
  /* 49 */ "store",
  /* 50 */ "store",
  /* 51 */ "move",
  /* 52 */ "jump",
  /* 53 */ "jr",
  /* 54 */ "mmult",
  /* 55 */ "mtoi",

  /* 56 */ "normi",
  /* 57 */ "nop",
  /* 58 */ "load",
  /* 59 */ "load",
  /* 60 */ "store",
  /* 61 */ "store",
  /* 62 */ "sat24",
  /* 63 */ "pack"
};

#define READ16(addr) ((*(addr)<<8)|*(addr+1))

const char *condTxt(int condCode)
{
  switch( condCode ){
  case 0:
    return "";
  case 1:
    return "nz,";
  case 2:
    return "z,";
  case (4|16):
    return "pl,";
  case (8|16):
    return "mi,";
  case 4:
    return "nc,";
  case 8:
    return "c,";
  case 0x15:
    return "gt,";
  case 0x16:
    return "pl_z,";
  case 0x19:
    return "mi_nz,";
  case 0x1a:
    return "mi_z,";
  }
  return "XX,";
}

int decode(const uint8_t *addr, uint32_t address, char *decoded)
{
  uint16_t opcode;
  int insn;
  int reg1; /* src */
  int reg2; /* dst */
  int offset;
  char help[16];
  opcode = READ16(addr);

  help[0] = 0;
  offset = 2;
  insn = (opcode >> 10);
  reg1 = (opcode >> 5) & 31;
  reg2 = opcode & 31;
  switch( insn ){
  case opcode_ADD:
  case opcode_ADDC:
  case opcode_SUB:
  case opcode_SUBC:
  case opcode_AND:
  case opcode_OR:
  case opcode_CMP:
  case opcode_DIV:
  case opcode_IMACN:
  case opcode_IMULT:
  case opcode_IMULTN:
  case opcode_MMULT:
  case opcode_MOVE:
  case opcode_MOVEFA:
  case opcode_MOVETA:
  case opcode_MTOI:
  case opcode_MULT:
  case opcode_NORMI:
  case opcode_ROR:
  case opcode_SH:
  case opcode_SHA:
  case opcode_XOR:
    sprintf(help,"r%d,r%d",reg1,reg2);
    break;
  case opcode_ABS:
  case opcode_NEG:
  case opcode_NOT:
  case opcode_RESMAC:
  case opcode_SAT8:
  case opcode_SAT16:
  case opcode_SAT24:
    sprintf(help,"r%d",reg2);
    break;
  case opcode_LOAD:
  case opcode_LOADB:
  case opcode_LOADW:
  case opcode_LOADP:
    sprintf(help,"(r%d),r%d",reg1,reg2);
    break;
  case opcode_STORE:
  case opcode_STOREB:
  case opcode_STOREW:
  case opcode_STOREP:
    sprintf(help,"r%d,(r%d)",reg2,reg1);
    break;
  case opcode_LOAD_R14:
    sprintf(help,"(R14+%d),r%d",reg1*4,reg2);
    break;
  case opcode_LOAD_R15:
    sprintf(help,"(R15+%d),r%d",reg1*4,reg2);
    break;
  case opcode_LOAD_R14_RN:
    sprintf(help,"(R14+r%d),r%d",reg1,reg2);
    break;
  case opcode_LOAD_R15_RN:
    sprintf(help,"(R15+r%d),r%d",reg1,reg2);
    break;
  case opcode_STORE_R14:
    sprintf(help,"r%d,(R14+%d)",reg2,reg1*4);
    break;
  case opcode_STORE_R15:
    sprintf(help,"r%d,(R15+%d)",reg2,reg1*4);
    break;
  case opcode_STORE_R14_RN:
    sprintf(help,"r%d,(R14+r%d)",reg2,reg1);
    break;
  case opcode_STORE_R15_RN:
    sprintf(help,"r%d,(R15+r%d)",reg2,reg1);
    break;
  case opcode_MOVE_PC:
    sprintf(help,"PC,r%d",reg2);
    break;
  case opcode_ADDQ:
  case opcode_ADDQT:
  case opcode_SUBQ:
  case opcode_SUBQT:
    if ( reg1 == 0 ) {
      reg1 = 32;
    }
  case opcode_BCLR:
  case opcode_BSET:
  case opcode_BTST:
  case opcode_CMPQ:
  case opcode_MOVEQ:
  case opcode_RORQ:
  case opcode_SHARQ:
  case opcode_SHLQ:
  case opcode_SHRQ:
    sprintf(help,"#%d,r%d",reg1,reg2);
    break;
  case opcode_NOP:
    *help = 0;
    break;
  case opcode_MOVEI:
    sprintf(help,"#$%08x,r%d", (READ16(addr+4)<<16)|READ16(addr+2),reg2);
    offset = 6;
    break;
  case opcode_JUMP:
    sprintf(help,"%s(r%d)",condTxt(reg2),reg1);
    break;
  case opcode_JR:
      {
        reg1 = (reg1 > 15) ? -31+reg1 : reg1;
        int addr = address + reg1*2;
        sprintf(help,"%s$%06x",condTxt(reg2),addr);
      }
    break;
  }

  if ( insn != 63 ){
    sprintf(decoded,"%s %s",opcodesTomTxt[insn],help);
  } else {
    if ( reg1 == 1 ){
      sprintf(decoded,"UNPACK %d",reg2);
    } else if ( reg1  == 0 ){
      sprintf(decoded,"PACK %d",reg2);
    } else {
      sprintf(decoded,"UNDEF %d,%d",reg1,reg2);
    }
  }
  return offset;
}

int
main (int argc, char *argv[])
{
  FILE *fileHandle;
  size_t fileSize;
  char *fileBuffer;
  int error;

  fileHandle = fopen(argv[1],"rb");
  if ( !fileHandle ){
    fprintf(stderr, "Cannot open %s\n", argv[1]);
    exit(-1);
  }
  fseek(fileHandle, 0, SEEK_END);
  fileSize = ftell(fileHandle);
  fseek(fileHandle, 0, SEEK_SET);

  fileBuffer = malloc(fileSize+3);

  error = fread(fileBuffer, 1, fileSize, fileHandle);

  fclose(fileHandle);

  if ( error != fileSize ){
    fprintf(stderr, "Could not read file\n");
    exit(-1);
  }

  uint32_t addr = 0xf03000;
  char help[64];
  int offset;
  const uint8_t *p = (uint8_t *)fileBuffer;
  if ( fileBuffer[0] == 'B' && fileBuffer[1] == 'S' ) {
    addr = (p[4]<<24)|(p[5]<<16)|(p[6]<<8)|p[7];
     p = (uint8_t *)(fileBuffer+12);
  }
  while( (char *)p < fileBuffer+fileSize ){
    offset = decode(p, addr, help);
    if ( offset == 2 ){
      printf("%08x: %04x           %s\n", addr, READ16(p), help);
    } else {
      printf("%08x: %04x %04x %04x %s\n",
             addr, READ16(p), READ16(p+2), READ16(p+4), help);
    }
    addr += offset;
    p += offset;
  }

  free(fileBuffer);

  return 0;
}
