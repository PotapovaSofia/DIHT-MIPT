#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
	
#define PORT 6666
#define BUFSIZE 1024
#define CLIENTS_CNT 10

char nicknames[CLIENTS_CNT][BUFSIZE] = {'\0'};
int nick = 0;
int nick_len[BUFSIZE];

void abort_prg(const char* err){
        perror(err);
        exit(1);
}

void send_msg(int receiver, int i, int sock, int size, char *recv_buf, fd_set *master) {
	if (FD_ISSET(receiver, master)){ //тестирование дескриптора
		if (receiver != sock && receiver != i) {
			if(-1 == send(receiver, nicknames[i], nick_len[i], 0) ||
			(-1 == send(receiver, ": ", 2, 0)) ||
			(-1 == send(receiver, recv_buf, size, 0)))
				abort_prg("send");
		}
	}
}
		
void send_recv(int i, fd_set *master, int sock, int n) {
	int size;
	char recv_buf[BUFSIZE];	
	if (0 >= (size = recv(i, recv_buf, BUFSIZE, 0))) { 
		if (size == 0) {
			printf("socket %d end up\n", i);
		}else
			abort_prg("recv error");
		memset(nicknames[i], '\0', strlen(nicknames[i]));
		close(i);
		FD_CLR(i, master);//удаляет файловый дескриптор из набора
	} else {
		//дадим имечко 
		if(nicknames[i][0] == '\0'){
			sprintf(nicknames[i], "%d", ++nick);
			nick_len[i] = strlen(nicknames[i]);
		}
		char* buf = malloc(strlen(recv_buf) + 1);
		if(buf == NULL)
			abort_prg("malloc error;");
		sscanf(recv_buf, "%s", buf);
		if(strcmp(buf, "/send") == 0){
			char nick_to[16];
			if(1 == sscanf(recv_buf + strlen(buf), "%15s", nick_to)){
				char* msg = strdup(recv_buf + strlen("/send") + strlen(nick_to) + 2);
				if(msg == NULL)
					abort_prg("malloc error");
				printf("%s\n", msg);
				printf("%s\n", nick_to);
				int k, ch = -1;
				for(k = 0; k < n+1; ++k){
					if(0 == strcmp(nicknames[k], nick_to))
						ch = k;
				}
				if(-1 == ch){
					printf("Wrong nickname\n");
					exit(0);
					//continue;
				}
				send_msg(/*nick_to*/ch, i, sock, strlen(msg), msg, master);
				free(msg);
			}
		}else if(strcmp(buf, "/nick") == 0){
			char new_nick[16];
			if(1 == sscanf(recv_buf + strlen(buf), "%15s", new_nick)){
				memset(nicknames[i], '\0', strlen(nicknames[i]));
				strcpy(nicknames[i], new_nick);
				nick_len[i] = strlen(new_nick);
			}
		} else {
			int j;
			for(j = 0; j < n+1; ++j)
				send_msg(j, i, sock, size, recv_buf, master);
		}
		free(buf);
	}	
}
		
void connection_accept(fd_set *master, int *n, int sock, struct sockaddr_in *client_addr) {
	socklen_t addrlen;
	int newsock;

	addrlen = sizeof(struct sockaddr_in);
	if(-1 == (newsock = accept(sock, (struct sockaddr *)client_addr, &addrlen)))
		abort_prg("accept");
	else {
		FD_SET(newsock, master);
		if(newsock > *n)
			*n = newsock;
		printf("new connection from %s on port %d \n",inet_ntoa(client_addr->sin_addr)/*преобразует ip-адрес в строку*/, ntohs(client_addr->sin_port)/*преобразует сетевой порядок расположения байтов в узловой*/);
	}
}

int main(int argc, char* argv[]) {
	int sock = 0;
	struct sockaddr_in my_addr, client_addr;
	
	fd_set master, rfds;
	FD_ZERO(&master);
	FD_ZERO(&rfds);
	int flag = 1;
        if (-1 == (sock = socket(AF_INET, SOCK_STREAM, 0)))
                abort_prg("socket error");
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(PORT);
        my_addr.sin_addr.s_addr = INADDR_ANY;
        memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

        if (-1 == setsockopt(sock, SOL_SOCKET/*level*/, SO_REUSEADDR, &flag, sizeof(int))) //устанавливает флаги на сокете: 
                abort_prg("setsockopt error");

        if (-1 == bind(sock, (struct sockaddr *)&my_addr, sizeof(my_addr))) //связывание сокета с адресом
                abort_prg("bind error");
    
        if (-1 == listen(sock, CLIENTS_CNT)) //сокет переводится в режим ожидания запросов со стороны клиента
                abort_prg("listen");
        printf("\nWaiting for client\n");
        fflush(stdout); //принудительная запись буф данных в stdout

	FD_SET(sock, &master);//добавление дскриптора
	int n = sock;
	while(1){
		rfds = master;
		if(-1 == select(n+1, &rfds, NULL, NULL, NULL))
			abort_prg("select error");		
		int i;
		for (i = 0; i < n+1; ++i) {
			if (FD_ISSET(i, &rfds)){//тестирование дескриптора
				if (i == sock)
					connection_accept(&master, &n, sock, &client_addr);
				else
					send_recv(i, &master, sock, n);
			}
		}
	}
	return 0;
}


