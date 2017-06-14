#ifndef TOKEN_H
#define TOKEN_H

#include <string>

using std::string;

#include <iostream>

using std::ostream;

/* Token types */

#define REG    0
#define OPCODE 1
#define TEXT   2
#define ASCII  3
#define ASCIIZ 4
#define INT    5
#define STR    6
#define ID     7
#define COLON  8
#define COMA   9

/* Opcodes */

#define ADD     0x0
#define SUB     0x1
#define MUL     0x2
#define DIV     0x3
#define FADD    0x4
#define FSUB    0x5
#define FMUL    0x6
#define FDIV    0x7
#define AND     0x8
#define OR      0x9
#define XOR     0xA
#define NOT     0xB
#define LB      0xC
#define LW      0xD
#define SB      0xE
#define SW      0xF
#define LI      0x10
#define B       0x11
#define BEQZ    0x12
#define BLTZ    0x13
#define SYSCALL 0x14
#define MOV     0x15

class Token {

  friend ostream& operator<<(ostream& out, const Token& t);

public:
  Token(unsigned char type = 0, int numval = 0, int line_num = 0, 
	char *strval = NULL);
  unsigned char getType() const;
  int getNum() const;
  char* getString() const;
  int getLineNum() const;
  
private:
  unsigned char type;
  int numval;
  char* strval;
  int line_num;
};

#endif
