/*  For AURIX version
 *
 *  remote ip & port : 192.168.0.21 : 40050
 *  local  ip & port : 192.168.0.20 : 50001 
 *  max. buffer number : 1460
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#define BUFFER_NUM 14641
#define SERVER_PORT 40050
#define SERVER_IP "192.168.0.21"

int main()
{
	int sockfd,i,RX_byte,num=0;		
	struct sockaddr_in dest;
	char buffer[BUFFER_NUM];
	char resp[BUFFER_NUM]={0};//"client_ack";
	
	/* initialize value in dest */
	bzero(&dest, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(SERVER_PORT);
	dest.sin_addr.s_addr = inet_addr(SERVER_IP);

	//while(1){

		/* create socket */
		sockfd = socket(PF_INET, SOCK_STREAM, 0);
	
		/* Connecting to server */
		connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));

		/* Receive message from the server and print to screen */
		bzero(buffer, BUFFER_NUM);
		send(sockfd,resp,sizeof(resp),0);
		printf("send complete.\n");
		RX_byte=recv(sockfd, buffer, sizeof(buffer), 0);
		printf("recv complete.\n");
		
		//if(RX_byte == BUFFER_NUM) num++;	
		printf("receive from server: %s RX bytes:%d count:%d\n", buffer,RX_byte,num);

		/* Close connection */
		close(sockfd);
	//}//while

	return 0;
}
