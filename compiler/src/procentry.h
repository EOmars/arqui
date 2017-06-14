#ifndef PROCENTRY_H
#define PROCENTRY_H

#include <iostream>

using std::ostream;

class ProcEntry {

  friend ostream& operator<<(ostream& out, const ProcEntry& p);

public:
  ProcEntry(char *id, int addr);
  char* getId() const;
  int getAddr() const;

private:
  char *procid;
  int address;
};

#endif
