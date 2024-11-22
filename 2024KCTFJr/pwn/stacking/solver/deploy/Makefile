main : main.o
	gcc -o main main.o -fno-stack-protector -no-pie

main.o : main.c
	gcc -c -o main.o main.c -fno-stack-protector 	-no-pie

clean :
	rm *.o main