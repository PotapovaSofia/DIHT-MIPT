#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define BUFSIZE 1024
//#define LOCAL_IP "127.0.0.1"
		
void abort_prg(const char* err){
	perror(err);
	exit(1);
}

void send_recv(int i, int sock, int fd) {
	char send_buf[BUFSIZE];
	char recv_buf[BUFSIZE];
	
	if(i == 0){
		fgets(send_buf, BUFSIZE, fd[0]/*stdin*/);
		//if (strcmp(send_buf, "/quit\n") == 0) {
		//	exit(0);
		//}else
			if(-1 == send(sock, send_buf, strlen(send_buf), 0))
				abort_prg("send error");
	} else {
		int size = recv(sock, recv_buf, BUFSIZE, 0);
		if(size == 0)
			abort_prg("connecting error");
		recv_buf[size] = '\0';
		printf("%s\n" , recv_buf);
		fflush(stdout);//принудительно записывает все буфф данные в stdout
	}
}

int main(int argc, char* argv[]) {
	char* ip;
	ip = "127.0.0.1";
	char* port;
	if(1 != sscanf(argv[2], "%s", port))
		abort_prg("incorrect ip address");
	int sock; //сокет
	struct sockaddr_in addr; //адрес	
	if (-1 == (sock = socket(AF_INET/*ipv4*/, SOCK_STREAM/*чтение и запись*/, 0))) 
                abort_prg("socket error");
        addr.sin_family = AF_INET; //семейство адресов
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);
        memset(addr.sin_zero, '\0', sizeof addr.sin_zero); 
        if(-1 == connect(sock, (struct sockaddr *)&addr, sizeof(addr)))  
                abort_prg("error in connect");

	fd_set master, rfds;
	FD_ZERO(&master);//инициализация дескрипторов?
        FD_ZERO(&rfds);
        FD_SET(0, &master);//добавление дескрипторов
        FD_SET(sock, &master);
	int n = sock;
/*	
	int fd[2];
	pipe(fd);
	//char[BUFSIZE] buf;
	dup2(fd[1], fileno(stdout));
	execl("/bin/sh", "sh", "-c", argv[1], NULL);
	//if(BUFSIZE != read(fd, &buf, BUFSIZE))
	//	abort_prg("Read_error");

*/
	while(1){
		rfds = master;
		if(-1 == select(n+1, &rfds/*массив дескрипторов*/, NULL, NULL, NULL)) //обрабатвает несколько файловых дескрипторов одновременно, очень полезная штука
			abort_prg("error in select");
		int i;		
		for(i=0; i < n+1; ++i)
			if(FD_ISSET(i, &rfds)) //тестирование дескрипора
				send_recv(i, sock);
	}
	close(sock);
	return 0;
}


