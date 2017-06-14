#include "parser.h"

AsmParser::AsmParser(deque<Token>& tokens){
  this -> tokens = tokens;
  currToken = tokens.front();
  pErrors = false;
  sErrors = false;
  tokensLeft = true;
  counter = PREAMBLESIZE;
}

void AsmParser::start(){
  if(currToken.getType() == TEXT){
    nextToken();
    codeStart = counter;
    do {
      sub();
    } while(tokensLeft && !pErrors);
    return;
  } else {
    do {
      dec();
    } while(currToken.getType() != TEXT  && !pErrors);
  }
  nextToken();
  if(pErrors){
    return;
  }
  codeStart = counter;
  do {
    sub();
  } while(tokensLeft && !pErrors);
}

void AsmParser::dec(){
  char *id, *strval;
  unsigned char type;
  int val;

  switch(currToken.getType()){
  case ASCII:
    type = currToken.getType();
    nextToken();
    if(currToken.getType() == ID){
      id = currToken.getString();
      nextToken();
      if(currToken.getType() == INT){
	val = currToken.getNum();
	if(!duplicates(id)){
	  StrEntry s(id, NULL, val, type == ASCIIZ, counter);
	  counter += val;
	  strings.push_back(s);
	} else {
	  semantError("Duplicated string identifier");
	}
	nextToken();
      } else {
	parseError(INT);
	return;
      }
    } else {
      parseError(ID);
      return;
    }
    break;
  case ASCIIZ:
    type = currToken.getType();
    nextToken();
    if(currToken.getType() == STR){
      strval = currToken.getString();
      nextToken();
      if(currToken.getType() == ID){
	id = currToken.getString();
	if(!duplicates(id)){
	  int size = strlen(strval);
	  StrEntry s(id, strval, size, type == ASCIIZ, counter);
	  counter += size + 1;
	  strings.push_back(s);
	} else {
	  semantError("Duplicated string identifier");
	}
	nextToken();
      } else {
	parseError(ID);
	return;
      }
    } else {
      parseError(STR);
      return;
    }
    break;
  default:
    parseError(ASCII);
    return;
  }
}

void AsmParser::sub(){
  char *id;

  if(currToken.getType() == ID){
    id = currToken.getString();
    nextToken();
    if(currToken.getType() == COLON){
      if(!duplicatep(id)){
	ProcEntry p(id, counter);
	procedures.push_back(p);
      } else {
	semantError("Duplicated procedure definition");
      }
      nextToken();
      do {
	instr();
      } while(currToken.getType() != ID && tokensLeft && !pErrors);
    } else {
      parseError(COLON);
      return;
    }
  } else {
    parseError(ID);
    return;
  }
}

void AsmParser::instr(){
  unsigned char opcode, dr, op1, op2;

  switch(currToken.getType()){
  case OPCODE:
    switch(currToken.getNum()){
    case ADD:
    case SUB:
    case MUL:
    case DIV:
    case FADD:
    case FSUB:
    case FMUL:
    case FDIV:
    case AND:
    case OR:
    case XOR:
      opcode = currToken.getNum();
      nextToken();
      if(currToken.getType() == REG){
	dr = currToken.getNum();
	nextToken();
	if(currToken.getType() == COMA){
	  nextToken();
	  if(currToken.getType() == REG){
	    op1 = currToken.getNum();
	    nextToken();
	    if(currToken.getType() == COMA){
	      nextToken();
	      if(currToken.getType() == REG){
		op2 = currToken.getNum();
		Instr instr(opcode, dr, op1, op2, 0, currToken.getLineNum());
		code.push_back(instr);
		counter += 4;
		nextToken();
	      } else {
		parseError(REG);
		return;
	      }
	    } else {
	      parseError(COMA);
	      return;
	    }
	  } else {
	    parseError(REG);
	    return;
	  }
	} else {
	  parseError(COMA);
	  return;
	}
      } else {
	parseError(REG);
	return;
      }
      break;
    case NOT:
    case LB:
    case SB:
    case LW:
    case SW:
    case MOV:
      opcode = currToken.getNum();
      nextToken();
      if(currToken.getType() == REG){
	dr = currToken.getNum();
	nextToken();
	if(currToken.getType() == COMA){
	  nextToken();
	  if(currToken.getType() == REG){
	    op2 = currToken.getNum();
	    if(opcode != MOV){
	      Instr instr(opcode, dr, 0, op2, 0, currToken.getLineNum());
	      code.push_back(instr);
	      counter += 4;
	    } else {
	      Instr instr1(OR, dr, op2, op2, 0, currToken.getLineNum());
	      code.push_back(instr1);
	      counter += 4;
	    }
	    nextToken();
	  } else {
	    parseError(REG);
	    return;
	  }
	} else {
	  parseError(COMA);
	  return;
	}
      } else {
	parseError(REG);
	return;
      }
      break;
    case LI:
      opcode = currToken.getNum();
      nextToken();
      if(currToken.getType() == REG){
	dr = currToken.getNum();
	nextToken();
	if(currToken.getType() == COMA){
	  nextToken();
	  if(currToken.getType() == INT){
	    Instr instr(opcode, dr, 0, 0, currToken.getNum(), currToken.getLineNum());
	    code.push_back(instr);
	    counter += 6;
	    nextToken();
	  } else if(currToken.getType() == ID){
	    Instr instr(opcode, dr, 0, 0, 0, currToken.getLineNum());
	    code.push_back(instr);
	    deferred.push_back(counter);
	    deferredid.push_back(currToken.getString());
	    counter += 6;
	    nextToken();
	  } else {
	    parseError(REG);
	    parseError(ID);
	    return;
	  }
	} else {
	  parseError(COMA);
	  return;
	}
      } else {
	parseError(REG);
	return;
      }
      break;
    case B:
      opcode = currToken.getNum();
      nextToken();
      if(currToken.getType() == REG){
	op2 = currToken.getNum();
	Instr instr(opcode, 0, 0, op2, 0, currToken.getLineNum());
	code.push_back(instr);
	counter += 4;
	nextToken();
      } else {
	parseError(REG);
	return;
      }
      break;
    case BEQZ:
    case BLTZ:
      opcode = currToken.getNum();
      nextToken();
      if(currToken.getType() == REG){
	dr = currToken.getNum();
	nextToken();
	if(currToken.getType() == COMA){
	  nextToken();
	  if(currToken.getType() == REG){
	    op2 = currToken.getNum();
	    Instr instr(opcode, dr, 0, op2, 0, currToken.getLineNum());
	    code.push_back(instr);
	    counter += 4;
	    nextToken();
	  } else {
	    parseError(REG);
	    return;
	  } 
	} else {
	  parseError(COMA);
	  return;
	}
      } else {
	parseError(REG);
	return;
      }
      break;      
    case SYSCALL:
      {
	Instr instr(currToken.getNum(), 0, 0, 0 ,0, currToken.getLineNum());
	code.push_back(instr);
	counter += 4;
	nextToken();
      }
      break;
    default:
      parseError(101);
      return;
    }
    break;
  default:
    parseError(OPCODE);
    return;
  }
}

Token AsmParser::nextToken(){
  tokens.pop_front();
  currToken = tokens.front();
  tokensLeft = !tokens.empty();
  return currToken;
}

string AsmParser::near(Token& t){
  switch(t.getType()){
  case REG:
    return "$reg";
  case OPCODE:
    switch(t.getNum()){
    case ADD:
      return "add";
    case SUB:
      return "sub";
    case MUL:
      return "mul";
    case DIV:
      return "div";
    case FADD:
      return "fadd";
    case FSUB:
      return "fsub";
    case FMUL:
      return "fmul";
    case FDIV:
      return "fdiv";
    case AND:
      return "and";
    case OR:
      return "or";
    case XOR:
      return "xor";
    case NOT:
      return "not";
    case LB:
      return "lb";
    case SB:
      return "sb";
    case LW:
      return "lw";
    case SW:
      return "sw";
    case LI:
      return "li";
    case B:
      return "b";
    case BEQZ:
      return "beqz";
    case BLTZ:
      return "bltz";
    case SYSCALL:
      return "syscall";
    default:
      cerr << "Opcode error, should be catched in the lexer" << endl;
      return "Invalid opcode";
    }
  case TEXT:
    return ".text";
  case ASCII:
    return ".ascii";
  case ASCIIZ:
    return ".asciiz";
  case INT:
    return "Integer literal";
  case STR:
    return t.getString();
  case ID:
    return t.getString();
  case COLON:
    return ":";
  case COMA:
    return ",";
  default:
    cerr << "Invalid token type" << endl;
    return "?";
  }
}

void AsmParser::semantError(const char* msg){
  cerr << msg << " on line: " << currToken.getLineNum() << endl;
  sErrors = true;
}

void AsmParser::parseError(unsigned char expected){
  switch(expected){
  case REG:
    cerr << "register identifier expected ";
    break;
  case OPCODE:
    cerr << "opcode keyword expected ";
    break;
  case TEXT:
    cerr << ".text keyword expected ";
    break;
  case ASCII:
    cerr << ".ascii or .asciiz keyword expected ";
    break;
  case INT:
    cerr << "integer literal expected ";
    break;
  case STR:
    cerr << "string literal expected ";
    break;
  case ID:
    cerr << "identifier expected ";
    break;
  case COLON:
    cerr << "':' expected ";
    break;
  case COMA:
    cerr << "',' expected ";
    break;
  case 100:
    cerr << "unexpected tokens left in line";
    break;
  case 101:
    cerr << "invalid opcode specification";    
  }
  cerr << " near " << near(currToken) << " at line: " 
       << currToken.getLineNum() << endl;
  pErrors = true;
}

deque<Instr>& AsmParser::codeGenerated(){
  return code;
}

deque<int>& AsmParser::deferredInstructions(){
  return deferred;
}

deque<ProcEntry>& AsmParser::procTable(){
  return procedures;
}

deque<StrEntry>& AsmParser::stringTable(){
  return strings;
}

bool AsmParser::duplicatep(char* id){
  std::deque<ProcEntry>::const_iterator it;
  for(it = procedures.begin(); it != procedures.end(); it++)
    if(strcmp((*it).getId(), id) == 0)
      return true;
  return false;
}

bool AsmParser::duplicates(char* id){
  std::deque<StrEntry>::const_iterator it;
  for(it = strings.begin(); it != strings.end(); it++)
    if(strcmp((*it).getId(), id) == 0)
      return true;
  return false;
}

bool AsmParser::parseErrors(){
  return pErrors;
}

bool AsmParser::semantErrors(){
  return sErrors;
}

int AsmParser::codeSize() const {
  return counter - codeStart;
}

deque <char*>& AsmParser::deferredIds(){
  return deferredid;
}
