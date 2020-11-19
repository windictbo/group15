
main: test.c cmd.o com.o
	gcc test.c cmd.o com.o -o main

cmd.o: cmd.c
	gcc -c cmd.c

com.o: com.c
	gcc -c com.c

clean: 
	rm *.o main
