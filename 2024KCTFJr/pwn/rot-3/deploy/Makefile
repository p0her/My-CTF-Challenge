main : main.o
	gcc -o main main.o -fno-stack-protector

main.o : main.c
	gcc -c -o main.o main.c -fno-stack-protector

clean :
	rm *.o main