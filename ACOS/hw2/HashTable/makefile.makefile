test: test.o libHashTable.so
	gcc -o test test.o -lHashTable -L.

test.o: test.c HashTable.h
	gcc -std=c99 -c -g test.c

libHashTable.so: HashTable.o
	gcc -shared -Wl,-soname,libHashTable.so HashTable.o -o libHashTable.so

HashTable.o: HashTable.c HashTable.h
	gcc -std=c99 -c -g HashTable.c -fPIC
	
