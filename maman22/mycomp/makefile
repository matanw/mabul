build:  mycomp.c complex.c complex.h command_parse.c command_parse.h common.h
	gcc mycomp.c -o mycomp.o -c -ansi -Wall -pedantic
	gcc complex.c -o complex.o -c -ansi -Wall -pedantic
	gcc command_parse.c -o command_parse.o -c -ansi -Wall -pedantic
	gcc -o mycomp mycomp.o complex.o command_parse.o

build_debug:  mycomp.c complex.c complex.h command_parse.c command_parse.h common.h
	gcc mycomp.c -o mycomp.o -g -c -ansi -Wall -pedantic
	gcc complex.c -o complex.o -g -c -ansi -Wall -pedantic
	gcc command_parse.c -o command_parse.o -g -c -ansi -Wall -pedantic
	gcc -o mycomp mycomp.o complex.o command_parse.o

clean: mycomp.c
	rm -f mycomp mycomp.o complex.o command_parse.o

