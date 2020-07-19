all: mp3

mp3: main.o add.o print.o freeList.o delete.o revPrint.o
	gcc -g main.o add.o print.o freeList.o revPrint.o delete.o -o mp3

main.o: main.c
	gcc -g -c main.c

add.o: add.c
	gcc -g -c add.c

print.o: print.c
	gcc -g -c print.c

revPrint.o: revPrint.c
	gcc -g -c revPrint.c

delete.o: delete.c
	gcc -g -c delete.c

freeList.o: freeList.c
	gcc -g -c freeList.c

clean:
	\rm *.o mp3
