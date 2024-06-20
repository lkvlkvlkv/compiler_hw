#include "node.hpp"
#include "typeSystem.hpp"

llvm::Type* TypeSystem::getVarType(const NIdentifier& type) const {
    return getVarType(type.name);
}

llvm::Type* TypeSystem::getVarType(std::string typeStr) const {
    if (typeStr.compare("int") == 0) {
        return intTy;
    } else if (typeStr.compare("float") == 0) {
        return floatTy;
    } else if (typeStr.compare("double") == 0) {
        return doubleTy;
    } else if (typeStr.compare("void") == 0) {
        return voidTy;
    }
    return nullptr;
}

void TypeSystem::addCast(llvm::Type* from, llvm::Type* to, llvm::CastInst::CastOps op) {
    _castTable[from][to] = op;
}