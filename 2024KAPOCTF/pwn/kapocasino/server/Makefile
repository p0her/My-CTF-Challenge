main: main.o
	gcc -o main main.o -lcrypto -O0
main.o: main.c 
	gcc -c -o main.o main.c -lcrypto -O0
clean:
	rm *.o main