#include "nty_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>

// #include <sys/socket.h>
#include <sys/types.h>


#define BUFFER_LENGTH 1024

int main() {
	nty_tcp_setup();

	usleep(1);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(12312);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0) {
		perror("bind");
		return 2;
	}
	
	printf("LISTEN CONFIGURING\n");
	if (listen(sockfd, 10) < 0) {
		return 3;
	}

	struct sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(struct sockaddr_in));
	socklen_t client_len = sizeof(client_addr);

	printf("START LISTEN");

	while(1){
		printf("LISTENING\n");
		int clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
		printf("ACCEPTED\n");
		char str[INET_ADDRSTRLEN] = {0};
		printf("recv from %s at port %d, sockfd:%d, clientfd:%d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
			ntohs(client_addr.sin_port), sockfd, clientfd);
		char buffer[BUFFER_LENGTH] = {0};
		int ret = recv(clientfd, buffer, BUFFER_LENGTH, 0);
		send(clientfd, buffer, ret, 0);
		buffer[ret] = '\0';
		printf("recv : %s\n", buffer);
	}

	return 0;
}
