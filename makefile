all: prog

prog: Utilities.o symbol_table.o opcode.o main.o assembler.o
	gcc -ansi -Wall -pedantic  main.o opcode.o symbol_table.o Utilities.o assembler.o -o prog

opcode.o: opcode.c opcode.h bool.h symbol_table.h Utilities.h Enums.h
	gcc -ansi -Wall -pedantic -c opcode.c

symbol_table.o: symbol_table.c symbol_table.h bool.h
	gcc -ansi -Wall -pedantic -c symbol_table.c

Utilities.o: Utilities.c Utilities.h Enums.h bool.h symbol_table.h
	gcc -ansi -Wall -pedantic -c Utilities.c

assembler.o: assembler.c assembler.h bool.h symbol_table.h
	gcc -ansi -Wall -pedantic -c assembler.c

main.o: main.c assembler.h bool.h
	gcc -ansi -Wall -pedantic -c main.c 

clean:
	rm *.o *~prog

