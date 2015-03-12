test: test.o libthreadpool.so
	gcc -o test test.o -L. -lthreadpool -Wl,-rpath,. -lrt -pthread

test.o: test.c
	gcc -c test.c

libthreadpool.so: threadpool.o
	gcc -shared -o libthreadpool.so threadpool.o

threadpool.o: threadpool.c
	gcc -fPIC -c threadpool.c -lrt -pthread

clean:
	rm libthreadpool.so threadpool.o test.o test 
