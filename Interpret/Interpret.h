#include "AST/AST.h"

class Interpretator {
  public:
    Interpretator(AST::ASTContext &ast) : ast_(ast) {}
    void run();

  private:
    AST::ASTContext &ast_;
};
