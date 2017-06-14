#ifndef STRENTRY_H
#define STRENTRY_H

#include <iostream>

using std::ostream;

class StrEntry {

  friend ostream& operator<<(ostream& out, const StrEntry& e);

public:
  StrEntry(char* id, char* str, int size, bool attachNull, int addr);
  char* getId() const;
  char* getStr() const;
  bool attach() const;
  int getAddr() const;
  int getSize() const;

private:
  char *id;
  char *str;
  bool withNull;
  int addr, size;
};

#endif
