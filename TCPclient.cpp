/*
 * Function version
 * 
 * 1.�b Windows �ϥ� Socket �ݭn link Winsock Library�C[client��]
 *   link�覡�G
 *   �M�ת��u�ݩʡv->�u�պA�ݩʡv->�u�s�����v->�u��J�v->�u��L�̩ۨʡv�[�J wsock32.lib ��  Ws2_32.lib
 *   �]�i�H�b�{�����A�ϥΥH�U�覡�[�J
 *   #pragma comment(lib, "wsock32.lib") �� #pragma comment(lib, "Ws2_32.lib")
 * 
 * 2.wsock32.lib �M Ws2_32.lib ���ϧO�G
 *   wsock32.lib �O���ª� 1.1 �����AWs2_32.lib �O���s�� 2.0 �����C
 *   wsock32.lib �� winsock.h �@�_�ϥΡAWs2_32.lib �� WinSock2.h �@�_�ϥΡC
 *   winsock.h �M WinSock2.h ����P�ɨϥΡAWinSock2.h �O�ΨӨ��N winsock.h�A�Ӥ��O�X�i winsock.h�C 
 * ================================20160428==================================================  
 * 3. Max. 1460 bytes
 * 4. Add package_struct, and float,int,.etc array in structure can send and receive correctly
 *
 *
 */

#define WIN32_LEAN_AND_MEAN
#include"TCPclient.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

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

WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo *result = NULL,
                *ptr = NULL,
                hints;


				
/********* Global Variables *********/


int TCPclientInit(void){
	QueryPerformanceFrequency(&timeus);
	//memset(recvbuf,0,sizeof(recvbuf));
	//memset(sendbuf,0,sizeof(sendbuf));

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
		system("pause");
        return 2;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;	
	
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
		
		pkg_tx.TOFarray[i]=rand();	//rand() % (�̤j��-�̤p��+1) ) + �̤p��
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


