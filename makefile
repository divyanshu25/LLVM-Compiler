compiler: driver.o lexer.o parser.o ast.o symbol.o CodeGeneration.o
	gcc -o compiler driver.o lexer.o parser.o ast.o symbol.o CodeGeneration.o -g
lexer.o:
	gcc -c lexer.c -g
driver.o:
	gcc -c driver.c -g
parser.o:
	gcc -c parser.c -g
ast.o:
	gcc -c ast.c -g
symbol.o:
	gcc -c symbol.c -g
CodeGeneration.o:
	gcc -c CodeGeneration.c -g
rm:
	rm -rf *.o
	rm *~
compile:
	nasm -f elf sourceCode.asm
	ld -m elf_i386 -s -o sourceCode sourceCode.o
	./sourceCode
