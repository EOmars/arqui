#include "procentry.h"

ProcEntry::ProcEntry(char *id, int addr){
  procid = id;
  address = addr;
}

char* ProcEntry::getId() const {
  return procid;
}

int ProcEntry::getAddr() const {
  return address;
}

ostream& operator<<(ostream& out, const ProcEntry& p){
  out << "SubID: " << p.procid << " Address: " << p.address;
  return out;
}
