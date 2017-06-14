#include "strentry.h"
#include <cstring>

StrEntry::StrEntry(char* id, char* str, int size, bool attachNull, int addr){
  this -> id = id;
  this -> str = str;
  this -> withNull = attachNull;
  this -> addr = 0;
  if(str != NULL)
    this -> size = strlen(str);
  else
    this -> size = size;
  this -> addr = addr;
}

int StrEntry::getAddr() const {
  return addr;
}

char* StrEntry::getId() const {
  return id;
}

char* StrEntry::getStr() const {
  return str;
}

bool StrEntry::attach() const {
  return withNull;
}

int StrEntry::getSize() const {
  return size;
}

ostream& operator<<(ostream& out, const StrEntry& e){
  out << "StrID: " << e.getId() << " Value: ";
  if(e.getStr() != NULL)
    out << "\"" << e.getStr() << "\"";
  else 
    out << "NUL";
  out << " attach nul char: " << e.attach() << " addr: " << e.getAddr();
  out << " size: " << e.size;
  return out;
}
