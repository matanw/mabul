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
bits_operations.o: bits_operations.c bits_operations.h
	$(COMPILE_COMMAND) bits_operations.c

command_parse.o: command_parse.c command_parse.h
	$(COMPILE_COMMAND) command_parse.c

first_stage_simple_handlers.o: first_stage_simple_handlers.c first_stage_simple_handlers.h data_structures.h error_handling.h  command_parse.h  utils.h
	$(COMPILE_COMMAND) first_stage_simple_handlers.c

first_stage_operation_handlers.o: first_stage_operation_handlers.c first_stage_operation_handlers.h \
first_stage_operation_handlers_inner.h data_structures.h error_handling.h constants.h \
 command_parse.h utils.h bits_operations.h list.h
	$(COMPILE_COMMAND) first_stage_operation_handlers.c

deallocation.o: deallocation.c deallocation.h data_structures.h list.h
	$(COMPILE_COMMAND) deallocation.c

debug_utils.o: debug_utils.c debug_utils.h constants.h bits_operations.h data_structures.h
	$(COMPILE_COMMAND) debug_utils.c

error_handling.o: error_handling.c error_handling.h data_structures.h
	$(COMPILE_COMMAND) error_handling.c

first_stage.o: first_stage.c first_stage.h list.h data_structures.h error_handling.h  command_parse.h debug_utils.h bits_operations.h utils.h first_stage_simple_handlers.h first_stage_operation_handlers.h
	$(COMPILE_COMMAND) first_stage.c

files_writer.o: files_writer.c files_writer.h data_structures.h utils.h constants.h error_handling.h
	$(COMPILE_COMMAND) files_writer.c

list.o:list.c list.h
	$(COMPILE_COMMAND) list.c

utils.o: utils.h utils.c constants.h
	$(COMPILE_COMMAND) utils.c

second_stage.o: second_stage.c second_stage.h data_structures.h debug_utils.h bits_operations.h list.h constants.h utils.h error_handling.h
	$(COMPILE_COMMAND) second_stage.c

myassembly.o:myassembly.c first_stage.h deallocation.h
	$(COMPILE_COMMAND) myassembly.c

myassembly: myassembly.o bits_operations.o command_parse.o list.o first_stage.o debug_utils.o deallocation.o second_stage.o files_writer.o utils.o error_handling.o first_stage_simple_handlers.o first_stage_operation_handlers.o
	gcc -o myassembly myassembly.o bits_operations.o command_parse.o list.o first_stage.o debug_utils.o deallocation.o second_stage.o files_writer.o utils.o error_handling.o first_stage_simple_handlers.o first_stage_operation_handlers.o

clean:
	rm -f myassembly *.o
