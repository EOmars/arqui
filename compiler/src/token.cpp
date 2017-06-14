#include "token.h"

Token::Token(unsigned char type, int numval, int line_num, char *strval){
  this -> type = type;
  this -> numval = numval;
  this -> line_num = line_num;
  this -> strval = strval;
}

unsigned char Token::getType() const {
  return type;
}

int Token::getNum() const {
  return numval;
}

char* Token::getString() const {
  return strval;
}

int Token::getLineNum() const {
  return line_num;
}

ostream& operator<<(ostream& out, const Token& t){
  const char *tipo;
  switch(t.type){
  case REG:
    tipo = "Register";
    break;
  case OPCODE:
    tipo = "OpCode";
    break;
  case TEXT:
    tipo = ".text";
    break;
  case ASCII:
    tipo = ".ascii";
    break;
  case ASCIIZ:
    tipo = ".asciiz";
    break;
  case INT:
    tipo = "Number Literal";
    break;
  case STR:
    tipo = "String Literal";
    break;
  case ID:
    tipo = "Identifier";
    break;
  case COLON:
    tipo = "':'";
    break;
  case COMA:
    tipo = "','";
    break;
  }
  out << "Token type: " << tipo << " numval: " << t.numval
      << " line_num: " << t.line_num
      << " strval: ";
  if(t.strval != NULL)
    out << t.strval;
  else
    out << "NULL";
}
