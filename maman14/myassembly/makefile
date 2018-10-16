build:  myassembly.c myassembly.h bits_operations.c bits_operations.h command_parse.c command_parse.h constants.h list.c list.h first_stage.h first_stage.c debug_utils.c debug_utils.h
	gcc myassembly.c -o myassembly.o -c -ansi -Wall -pedantic
	gcc bits_operations.c -o bits_operations.o -c -ansi -Wall -pedantic
	gcc command_parse.c -o command_parse.o -c -ansi -Wall -pedantic
	gcc list.c -o list.o -c -ansi -Wall -pedantic
	gcc first_stage.c -o first_stage.o -c -ansi -Wall -pedantic
	gcc debug_utils.c -o debug_utils.o -c -ansi -Wall -pedantic
	gcc -o myassembly myassembly.o bits_operations.o command_parse.o list.o first_stage.o debug_utils.o

clean: myassembly.c
	rm -f myassembly myassembly.o
