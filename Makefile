CFLAGS = -pedantic -Wall
CC = clang

vm-debug : mathvm.c mathvm.h stack.c stack.h
	$(CC) $(CFLAGS) -D DEBUG_VM mathvm.c stack.c -o vm-debug

tree-debug : ast.c mathvm.h ast.h
	$(CC) $(CFLAGS) -D DEBUG_TREE ast.c -o tree-debug

parse-debug : compile.c compiler.h ast.c ast.h mathvm.h
	@# here we need -fwritable-strings because we try to parse a string
	@# literal on debug
	$(CC) $(CFLAGS) -fwritable-strings -D DEBUG_PARSE compile.c ast.c -o parse-debug

compile-debug : compile.c compiler.h ast.c ast.h mathvm.h
	@# here we need -fwritable-strings because we try to parse a string
	@# literal on debug
	$(CC) $(CFLAGS) -fwritable-strings -D DEBUG_COMPILE compile.c ast.c -o compile-debug

eval-debug : compile.c compiler.h ast.c ast.h mathvm.c mathvm.h stack.c stack.h interface.c compiler_interface.h
	@# here we need -fwritable-strings because we try to parse a string
	@# literal on debug
	$(CC) $(CFLAGS) -fwritable-strings -D DEBUG_EVAL compile.c ast.c mathvm.c stack.c interface.c -o eval-debug

math : compile.c compiler.h ast.c ast.h mathvm.c mathvm.h stack.c stack.h interface.c compiler_interface.h main.c
	$(CC) $(CFLAGS) compile.c ast.c mathvm.c stack.c interface.c main.c -o math

.PHONY : clean debug-vm debug-tree debug-parse debug-compile debug-eval

clean :
	rm debug-vm debug-tree debug-parse debug-compile debug-eval math

debug-vm : vm-debug
	./vm-debug

debug-tree : tree-debug
	./tree-debug

debug-parse : parse-debug
	./parse-debug

debug-compile : compile-debug
	./compile-debug

debug-eval : eval-debug
	./eval-debug
