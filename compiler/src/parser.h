#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "instr.h"
#include "strentry.h"
#include "procentry.h"

#include <cstring>
#include <deque>

using std::deque;

#include <string>

using std::string;

#include <iostream>

using std::cout;
using std::cerr;
using std::ostream;
using std::endl;

#define PREAMBLESIZE 10

class AsmParser {
public:
  AsmParser(deque<Token>& tokens);
  void start();
  deque<Instr>& codeGenerated();
  deque<int>& deferredInstructions();
  deque<char *>& deferredIds();
  deque<ProcEntry>& procTable();
  deque<StrEntry>& stringTable();
  bool parseErrors();
  bool semantErrors();
  int codeSize() const;

private:
  deque<Token> tokens;
  deque<Instr> code;
  deque<int> deferred;
  deque<char*> deferredid;
  deque<ProcEntry> procedures;
  deque<StrEntry> strings;
  int counter;
  int codeStart;

  Token currToken;
  bool pErrors;
  bool sErrors;
  bool tokensLeft;

  Token nextToken();
  void parseError(unsigned char expected);
  void semantError(const char *msg);
  string near(Token& t);
  bool duplicatep(char* id);
  bool duplicates(char* id);

  /* Non-terminals */
  void dec();
  void sub();
  void instr();
};

#endif
