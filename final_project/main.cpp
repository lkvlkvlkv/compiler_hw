#include <iostream>
#include "node.hpp"
#include <memory>
#include "CodeGen.hpp"
#include "ObjGen.hpp"

extern NProgram* programBlock;
extern int yyparse();

int main() {
    yyparse();
    CodeGenContext context;
    context.generateCode(*programBlock);
    ObjGen(context);
    return 0;
}