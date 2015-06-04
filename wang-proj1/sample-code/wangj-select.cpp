#include "wangj-headers.h"
#include <WinSock.h>
#include <time.h>
using namespace std;

//void recvnew(string &recv_string, int bytes, SOCKET sock, char buf[], int BUF_SIZE);
bool Receive(SOCKET sock, string &recv_string,int bytes, char buf[], int BUF_SIZE ) 
{
	FD_SET Reader;
	FD_ZERO(&Reader);
	FD_SET( sock, &Reader);    // add your sock to the set Reader    
	struct timeval timeout;  // set timeout, used for select()
	timeout.tv_sec = 10;    // must include <time.h>
	timeout.tv_usec = 0; 
	recv_string = "";  // empty string
	//bytes = 0;
    //string s1;
	do
	{
		if ( select(0, &Reader, NULL, NULL, &timeout) > 0 )
		{	
			if ((bytes = recv (sock, buf, BUF_SIZE-1, 0))==SOCKET_ERROR )
			{
				printf ("no data received: %d\n", WSAGetLastError ());
				break; 
			}
			else if ( bytes> 0)
			{
				buf[bytes]=0; // NULL terminate buffer
				recv_string += buf; //append to the string
			} 				
		} 
		else
		{ 
			printf( "timeout \n"); 
			return false;
		}
	}while ( bytes>0 ); // finish receiving response
	//cout << recv_string <<endl;
	return true; 
}