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
 
/******* Performance test ***********/
#include <windows.h>
#define timeNum 5
#define timerStart(x)	QueryPerformanceCounter(&start[x]);	
#define timerStop(x)	QueryPerformanceCounter(&end[x]);TimeSave[x] = 1000*(end[x].QuadPart-start[x].QuadPart)/(double)(timeus.QuadPart);
/******* Performance test ***********/


/********* Extern Variables *********/
extern LARGE_INTEGER start[timeNum], end[timeNum], timeus;				
extern double TimeSave[timeNum];

struct CARINFO
{
	int x,y;	//Car coordinate	
	float angle;//Include angle between y-axis and car direction
		
};


/********* Extern Variables *********/

int TCPclientInit(void);
unsigned int TCPclientCommunication(unsigned char *sendbuf, int sendbufLen, unsigned char *recvdata);
int TCPtest(void);




