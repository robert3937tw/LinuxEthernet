/*
 * TCPclient.c
 *
 * Function version
 * For AURIX version
 *
 * remote ip & port : 192.168.0.21 : 40050  AURIX
 * local  ip & port : 192.168.0.20 : 50001  PC
 * 
 * ================================20160428==================================================  
 * 1. Max. 1460 bytes
 * 2. Add package_struct, and float,int,.etc array in structure can send and receive correctly
 *
 *
 */
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include"TCPclient.h"

//#define TX_BUFLEN 900	//max 1460 but "Aurix:  " occupi 8 bytes
#define RX_BUFLEN	100
#define ARRAY_LEN	14641
#define SERVER_PORT	40050
#define SERVER_IP	"192.168.0.21"


/********* Extern Variables *********/
struct timespec start[timeNum], end[timeNum];
double TimeSave[timeNum];
/********* Extern Variables *********/



/********* Global Variables *********/
struct sockaddr_in dest;
int sockfd;
int recvbuflen = RX_BUFLEN;
int iResult;
int i;


/********* Global Variables *********/


int TCPclientInit(void){
	
	/* initialize value in dest */
	bzero(&dest, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(SERVER_PORT);
	dest.sin_addr.s_addr = inet_addr(SERVER_IP);   
	printf("Initialize success!\n");
	PAUSE;

}

unsigned int TCPclientCommunication(unsigned char *sendbuf, int sendbufLen, unsigned char *recvdata){
	
	unsigned char* recvbuf = recvdata;
	unsigned int recvdataLen;
	i=0;

 	/* create socket */
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
        	printf("Create socket failed");
		PAUSE;
        	return 4;
        }
	//printf("Create socket success!\n");

	/* Connecting to server */
	iResult=connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));
        if (iResult < 0 ) {
            	close(sockfd);
            	printf("Unable to connect to server!");
		PAUSE;
        	return 5;
        }
	//printf("Connecting to server success!\n");
	
    	/* Send an initial buffer */
timerStart(0);
	iResult = send( sockfd, (char*)sendbuf, sendbufLen, 0 );
timerStop(0);
	if (iResult < 0 ) {
	        close(sockfd);	
	        printf("Send failed");
		PAUSE;
        	return 6;
    	}
	
    	//printf("Bytes sent: %d size:%d \n", iResult,sendbufLen);
	//for(i=0;i<iResult;i+=4)
	//		printf("%X %X %X %X | ",*(sendbuf+i),*(sendbuf+i+1),*(sendbuf+i+2),*(sendbuf+i+3));


	// shutdown the connection since no more data will be sent
	iResult = shutdown(sockfd, SHUT_WR);
	if (iResult < 0 ) {
	        close(sockfd);
        	printf("shutdown failed");
                return 7;
    	}


    // Receive until the peer closes the connection
	recvdataLen=0;
timerStart(1);
	iResult = recv(sockfd, (char*)recvbuf, recvbuflen, 0);
/*
	do {
	        iResult = recv(sockfd, (char*)recvbuf, recvbuflen, 0);
	        if ( iResult > 0 ){
			//for(i=0;i<iResult;i+=4)
			//printf("%X %X %X %X \n",*(recvbuf+i),*(recvbuf+i+1),*(recvbuf+i+2),*(recvbuf+i+3));
			//printf("RX: %4d bytes\n", iResult);
			
			recvbuf += iResult;		//shift buffer pointer
			recvdataLen += iResult;	//calculate total number of receive bytes
			
			
		}//iResult > 0
	        else if ( iResult == 0 ){
	        	//printf("Connection closed\n");
			//printf("Total bytes received: %u \n", recvdataLen);
		}
        	else
            		printf("recv failed with error");

    } while( iResult > 0 );
*/
timerStop(1);

    	// Close the SOCKET
    	close(sockfd);
	//return recvdataLen; 		
	return iResult;
}

#ifdef TCPtest
int main(void){
	
	struct CARINFO CarInfo;
	unsigned int 	rxBytes,
			count=0;
	unsigned char globalmap[14641];


	double max=0,min=999,total=0;

	TCPclientInit();
  		
while(count<1000){
	for(i=0;i<14641;i++)
		globalmap[i]=0;

	
	//create test data
	for(i=0;i<3660;i++)
		globalmap[i]=i;
timerStart(2);	
	rxBytes = TCPclientCommunication(globalmap, sizeof(globalmap), (unsigned char*)&CarInfo);
timerStop(2);	


	printf("RX : %u\n",rxBytes);
	printf("Car x:%d y:%d angle:%.3f\n",CarInfo.x, CarInfo.y, CarInfo.angle);
	printf("send:%.3lf ms, recv:%.3lf ms, ",TimeSave[0],TimeSave[1]);
	printf("Total:%.3lf ms\n", TimeSave[2]);
	if(++count>0){
		if(TimeSave[2]>=max) max=TimeSave[2];
		if(TimeSave[2]<=min) min=TimeSave[2];
		total+=TimeSave[2];
		printf("max:%.3lf ms, min:%.3lf ms, avg:%.3lf ms, count:%d\n",max,min,(double)(total/(count)),count);
	}


	//PAUSE;
	
}//while    

	PAUSE;
    return 0;
}
#endif


