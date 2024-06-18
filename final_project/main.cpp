#include <iostream>
#include "node.hpp"
#include <memory>
// #include "CodeGen.hpp"


extern std::shared_ptr<NBlock> programBlock;
extern int yyparse();

int main() {
    yyparse();
    std::cout << programBlock << std::endl;

    // CodeGenContext context;
    // context.generateCode(*programBlock);

    return 0;
}