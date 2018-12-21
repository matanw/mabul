COMPILE_COMMAND=gcc -c -ansi -Wall -pedantic

complex.o:  complex.c complex.h
	$(COMPILE_COMMAND) complex.c

command_parse.o:  command_parse.c command_parse.h common.h
	$(COMPILE_COMMAND) command_parse.c

mycomp.o:  mycomp.c complex.h command_parse.h common.h
	$(COMPILE_COMMAND) mycomp.c

mycomp:  mycomp.o complex.o command_parse.o
	gcc -o mycomp mycomp.o complex.o command_parse.o

clean:
	rm mycomp
	rm *.o

