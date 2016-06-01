/*
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
#define RX_BUFLEN 100
#define ARRAY_LEN 14641
#define SERVER_PORT "40050"
#define SERVER_IP "192.168.0.21"

/*
struct package_struct
{
	unsigned char TOFarray[ARRAY_LEN];	
		
};
*/

/********* Extern Variables *********/
LARGE_INTEGER start[timeNum], end[timeNum], timeus;				
double TimeSave[timeNum];
/********* Extern Variables *********/



/********* Global Variables *********/
int i;
//char *sendbuf ;
//char sendbuf[TX_BUFLEN];	
//char recvbuf[RX_BUFLEN];
int iResult;
int recvbuflen = RX_BUFLEN;


SOCKET ConnectSocket = INVALID_SOCKET;

struct sockaddr_in dest;
struct addrinfo *result = NULL,
                *ptr = NULL,
                hints;


				
/********* Global Variables *********/


int TCPclientInit(void){
	QueryPerformanceFrequency(&timeus);
	//memset(recvbuf,0,sizeof(recvbuf));
	//memset(sendbuf,0,sizeof(sendbuf));

	/* initialize value in dest */
	bzero(&dest, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_port = htons(SERVER_PORT);
	dest.sin_addr.s_addr = inet_addr(SERVER_IP);   

}

unsigned int TCPclientCommunication(unsigned char *sendbuf, int sendbufLen, unsigned char *recvdata){
	
	unsigned char* recvbuf = recvdata;
	unsigned int recvdataLen;
	
	// Resolve the server address and port
    iResult = getaddrinfo(SERVER_IP, SERVER_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
		system("pause");
        return 3;
    }
	
	i=0;
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
		i++;
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
			system("pause");
            return 4;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        //break;
    }
	//printf("ptr try %d times\n",i);
    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server! %s:%s\n ",SERVER_IP,SERVER_PORT);
        WSACleanup();
		system("pause");
        return 5;
    }
	
    // Send an initial buffer
timerStart(0);
    iResult = send( ConnectSocket, (char*)sendbuf, sendbufLen, 0 );
timerStop(0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
		system("pause");
        return 6;
    }

	
    //printf("Bytes sent: %ld size:%d \n", iResult,sendbufLen);
	//for(i=0;i<iResult;i+=4)
	//		printf("%X %X %X %X \n",*(sendbuf+i),*(sendbuf+i+1),*(sendbuf+i+2),*(sendbuf+i+3));

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 7;
    }

    // Receive until the peer closes the connection
	recvdataLen=0;
timerStart(1);
    do {
        iResult = recv(ConnectSocket, (char*)recvbuf, recvbuflen, 0);
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
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );
timerStop(1);

    // Close the SOCKET
    closesocket(ConnectSocket);
	return recvdataLen; 		
}

/*
int TCPtest(void){
	
	struct package_struct pkg_tx,pkg_rx; 
	unsigned int 	diff_bytes=0,
					rxBytes,
					count=0;
	//double start,end;
	LARGE_INTEGER start, end, timeus;
	double thisTime,max=0,min=999,total=0,speed;
	
	
	QueryPerformanceFrequency(&timeus);
	
	srand(time(NULL));
	
	TCPclientInit();
    	
	
		
while(count<10000){
	
	//assign (random)
	for(i=0;i<ARRAY_LEN;i++){
		
		pkg_tx.TOFarray[i]=rand();	//rand() % (最大值-最小值+1) ) + 最小值
		//pkg_tx.TOFarray[i]= i;	
		//if((i%16==0)&&(i>0)) printf("num=%5d\n",i);
		//printf("%02X ",pkg_tx.TOFarray[i]);
		
	
	}//for 		
	//printf("\n");
	printf("\ncount:%u \n",++count);
	//start=clock();
    QueryPerformanceCounter(&start);
    
	rxBytes=TCPclientCommunication((unsigned char*)&pkg_tx, sizeof(pkg_tx), (unsigned char*)&pkg_rx);
	
    QueryPerformanceCounter(&end);
    //end=clock();	
	
	//memcpy(&pkg_rx,recvptr+8,sizeof(pkg_rx));//special case
	
	//diff_bytes=0;
	for(i=0;i<ARRAY_LEN;i++){
		if(pkg_rx.TOFarray[i] != pkg_tx.TOFarray[i]){
			diff_bytes++;
			printf("diff at index %4d, TX:%02X RX:%02X\n",i,pkg_tx.TOFarray[i],pkg_rx.TOFarray[i]);			
		}
			
		//if((i%16==0)&&(i>0)) printf("num=%5d\n",i);
		//printf("%02X ",pkg_rx.TOFarray[i]);
		
	}//for 
	
	
	thisTime = 1000*(end.QuadPart-start.QuadPart)/(double)(timeus.QuadPart);
	speed = 1000.0*rxBytes/1024/thisTime;

	printf("Total diff:%u ", diff_bytes);
	printf("This time:%lf ms\n", thisTime);
	if(count>1){
		if(thisTime>=max) max=thisTime;
		if(thisTime<=min) min=thisTime;
		total+=thisTime;
		printf("max:%lf ms, min:%lf ms, avg:%lf ms\n",max,min,(double)(total/(count-1)));
	}	
	
	//system("pause");
	
}//while    

	// cleanup
	WSACleanup();
	system("pause");
    return 0;
}
*/


