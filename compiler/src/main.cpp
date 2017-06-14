int main(int argc, char *argv[]) {
  ++argv; --argc;
  if(argc != 2){
    cout << "Invalid parameters.  Usage\nsasm <filename> <output>" << endl;
    return 127;
  } else {
    yyin = fopen(argv[0], "r");
    if(yyin == NULL){
      cerr << "Error, assembly file not found" << endl;
      return 126;
    }
  }
  yylex();
  if(lexer_stop){
    cerr << "Compilation halted due to lexical analysis errors" << endl;
    return 1;
  } else {
    AsmParser parser(tokens);
    parser.start();
    if(parser.parseErrors()){
      cerr << "Compilation halted due to syntax analysis errors" << endl;
      return 2;
    }
    if(parser.semantErrors()){
      cerr << "Compilation halted due to semantic analysis errors" << endl;
      return 3;
    }

    deque<StrEntry> strings = parser.stringTable();
    deque<ProcEntry> procedures = parser.procTable();
    deque<Instr> code = parser.codeGenerated();
    deque<int> deferred = parser.deferredInstructions();
    deque<char*> deferredid = parser.deferredIds();

    AsmCodeGen cg(code, deferred, deferredid,
		  procedures, strings,
		  parser.codeSize());

    cg.compile(argv[1]);

    if(cg.codeGenErrors()){
      cerr << "Compilation halted due to code generation errors" << endl;
      return 4;
    } else {
      cerr << "File succesfully compiled to: " << argv[1] << endl;
      return 0;
    }
  }
}
