#ifndef CODEGEN_H
#define CODEGEN_H

#include "procentry.h"
#include "strentry.h"
#include "instr.h"

#include <deque>

using std::deque;

#include <fstream>

using std::ofstream;
using std::ios;

#include <iostream>

using std::cerr;
using std::endl;

class AsmCodeGen {
public:
  AsmCodeGen(deque<Instr>& code,
	     deque<int>& deferred, 
	     deque<char*>& deferredid, 
	     deque<ProcEntry>& proc, 
	     deque<StrEntry>& strings,
	     int codeSize);
  void compile(const char* filename);
  bool cgenErrors() const;
  deque<StrEntry> strings;
  bool codeGenErrors() const;

private:
  deque<Instr> code;
  deque<int> deferred;
  deque<char*> deferredid;
  deque<ProcEntry> proc;

  char *strSection;
  char *codeSection;
  int strSectionSize;
  int codeSectionSize;
  int mainAddr;

  bool errors;  
  Instr current;

  void calculateProcAddresses();
  void nextInstr();
  int lookup(char *id);
  void write(ostream& out);
  void cgenErr(const char *msg, bool showLine);
  void buildDataSection();
  void buildCodeSection();
  void writeCode(const char* filename);
  bool findMain();
};

#endif
