#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main()
{
	int sockfd;
	struct sockaddr_in dest;
	char buffer[20] = "Hi client! ";
	char buffer1[10];
	/* create sockett */
	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	/* initialize structure dest */
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(50001);
	/* this line is different from client */
	dest.sin_addr.s_addr = INADDR_ANY;

	/* Assign a port number to socket */
	bind(sockfd, (struct sockaddr*)&dest, sizeof(dest));

	/* make it listen to socket with max 20 connections */
	listen(sockfd, 20);
    printf("Server is lisning...\n");



	/* infinity loop -- accepting connection from client forever */
		int clientfd;
		struct sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);
		client_addr.sin_port = htons(40050);

        /* Wait and Accept connection */
        clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

	while(1){
        /* Send message */

	send(clientfd, buffer, sizeof(buffer), 0);
        int res=recv(clientfd, buffer1, sizeof(buffer1), 0);
        printf("receive from client: %s, %d bytes\n", buffer1,res);
        //write(fd, buffer1, sizeof(buffer1));  
	}//while

        /* close(client) */
        close(clientfd);
        

    /* close(server) , but never get here because of the loop */
    close(sockfd);
    return 0;
}
