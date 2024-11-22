main : main.o
	gcc -o main main.o -no-pie

main.o : main.c
	gcc -c -o main.o main.c -no-pie

clean :
	rm *.o main