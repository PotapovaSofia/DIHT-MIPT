all: server client
server: server.c
	gcc -o server server.c -lrt
client: client.c
	gcc -o client client.c -lrt
clean:
	rm client server
