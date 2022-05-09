all: zsh

zsh: my_cp.o my_rm.o utils.o zsh.o
	gcc -o zsh my_cp.o my_rm.o utils.o myshell.o

my_cp.o: utils.o
	gcc -c my_cp.h my_cp.c

my_rm.o: utils.o
	gcc -c my_rm.h my_rm.c

zsh.o: utils.o
	gcc -c myshell.c

utils.o:
	gcc -c utils.h utils.c

install: all
clean:
	rm *.o && rm *.gch && rm zsh