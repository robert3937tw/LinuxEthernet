/*
 * TCPclient.h
 *
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

/* define something */ 
#define TCPtest



/* For Linux pause */
#define PAUSE printf("Press Enter key to continue..."); fgetc(stdin);  

/******* Performance test ***********/
#define timeNum 5
#define timerStart(x)	clock_gettime(CLOCK_MONOTONIC, &start[x]);
#define timerStop(x)	clock_gettime(CLOCK_MONOTONIC, &end[x]);TimeSave[x] = 1000*(end[x].tv_sec-start[x].tv_sec)+(end[x].tv_nsec-start[x].tv_nsec)/1000000.0;
/******* Performance test ***********/


/********* Extern Variables *********/
extern struct timespec start[timeNum], end[timeNum];
extern double TimeSave[timeNum];

struct CARINFO
{
	int x,y;	//Car coordinate	
	float angle;//Include angle between y-axis and car direction
		
};

/********* Extern Variables *********/




int TCPclientInit(void);
unsigned int TCPclientCommunication(unsigned char *sendbuf, int sendbufLen, unsigned char *recvdata);





