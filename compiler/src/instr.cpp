#include "instr.h"

Instr::Instr(unsigned char opcode, unsigned char dest,
	     unsigned char op1, unsigned char op2, int cval, int line){
  this -> opcode = opcode;
  this -> dest = dest;
  this -> op1 = op1;
  this -> op2 = op2;
  this -> cval = cval;
  this -> line_num = line;
}

int Instr::getLineNum() const {
  return line_num;
}

void Instr::setCval(int val){
  cval = val;
}

int Instr::getCval() const {
  return cval;
}

unsigned char Instr::getOpcode() const {
  return opcode;
}

unsigned char Instr::getOp1() const {
  return op1;
}

unsigned char Instr::getOp2() const {
  return op2;
}

unsigned char Instr::getDest() const {
  return dest;
}

ostream& operator<<(ostream& out, const Instr& i){
  const char* op;

  switch(i.getOpcode()){
  case 0: op = "add"; break;
  case 1: op = "sub"; break;
  case 2: op = "mul"; break;
  case 3: op = "div"; break;
  case 4: op = "fadd"; break;
  case 5: op = "fsub"; break;
  case 6: op = "fmul"; break;
  case 7: op = "fdiv"; break;
  case 8: op = "and"; break;
  case 9: op = "or"; break;
  case 10: op = "xor"; break;
  case 11: op = "not"; break;
  case 12: op = "lb"; break;
  case 13: op = "lw"; break;
  case 14: op = "sb"; break;
  case 15: op = "sw"; break;
  case 16: op = "li"; break;
  case 17: op = "b"; break;
  case 18: op = "beqz"; break;
  case 19: op = "bltz"; break;
  case 20: op = "syscall"; break;
  case 21: op = "mov"; break;
  }
  out << "Operation: " << op << "\tdr: " << (int) (i.getDest()) << "\top1: " 
      << (int) (i.getOp1()) << "\top2: " << (int) (i.getOp2());
  return out;
}
