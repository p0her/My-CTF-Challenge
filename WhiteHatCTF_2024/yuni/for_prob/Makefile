main : main.o
	gcc -o main main.o -lseccomp

main.o : main.c
	gcc -c -o main.o main.c -lseccomp

clean:
	rm *.o main