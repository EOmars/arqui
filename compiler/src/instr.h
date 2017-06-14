#ifndef INSTR_H
#define INSTR_H

#include <iostream>

using std::ostream;

class Instr {

  friend ostream& operator<<(ostream& out, const Instr& i);

public:
  Instr(unsigned char opcode, unsigned char dest,
	unsigned char op1, unsigned char op2, int cval, int line);
  void setCval(int val);
  unsigned char getOpcode() const;
  unsigned char getDest() const;
  unsigned char getOp1() const;
  unsigned char getOp2() const;
  int getCval() const;
  int getLineNum() const;

private:
  unsigned char opcode, dest, op1, op2;
  int cval;
  int line_num;
};
#endif
