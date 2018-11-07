COMPILE_COMMAND=gcc -c -ansi -Wall -pedantic

#TODO: make it hierercy
build:  myassembly.c bits_operations.c bits_operations.h command_parse.c command_parse.h constants.h list.c list.h first_stage.h first_stage.c debug_utils.c debug_utils.h deallocation.c deallocation.h data_structures.h files_writer.c files_writer.h second_stage.c second_stage.h
	$(COMPILE_COMMAND) myassembly.c
	$(COMPILE_COMMAND) bits_operations.c
	$(COMPILE_COMMAND) command_parse.c
	$(COMPILE_COMMAND) list.c
	$(COMPILE_COMMAND) first_stage.c
	$(COMPILE_COMMAND) second_stage.c
	$(COMPILE_COMMAND) files_writer.c
	$(COMPILE_COMMAND) debug_utils.c
	$(COMPILE_COMMAND) deallocation.c
	gcc -o myassembly myassembly.o bits_operations.o command_parse.o list.o first_stage.o debug_utils.o deallocation.o second_stage.o files_writer.o

clean: myassembly.c
	rm -f myassembly myassembly.o
