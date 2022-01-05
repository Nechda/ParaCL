cd Parser
bison Parser.y -o Parser.cc --defines=Parser.hh
cd ../
llvm_flags="`../install/bin/llvm-config --cxxflags --ldflags --system-libs --libs all`"
clang="../install/bin/clang++"

mkdir obj
$clang -c -w -g -I . main.cpp              $llvm_flags -o obj/main.o
$clang -c -w -g -I . AST/ASTAnalys.cpp     $llvm_flags -o obj/ASTAnalys.o
$clang -c -w -g -I . AST/ASTNode.cpp       $llvm_flags -o obj/ASTNode.o
$clang -c -w -g -I . Codegen/CGVisitor.cpp $llvm_flags -o obj/CGVisitor.o
$clang -c -w -g -I . Codegen/Codegen.cpp   $llvm_flags -o obj/Codegen.o
$clang -c -w -g -I . Parser/Parser.cc      $llvm_flags -o obj/Parser.o
$clang -c -w -g -I . Parser/lex.yy.cc      $llvm_flags -o obj/lex.o
$clang -c -w -g -I . Parser/Driver.cpp     $llvm_flags -o obj/Driver.o
$clang -c -w -g -I . Runtime/runtime.c     $llvm_flags -o obj/runtime.o

$clang obj/*.o $llvm_flags -o a.out