#include"wangj-headers.h"
using namespace std;
void recvnew(string &recv_string, int bytes, SOCKET sock, char buf[], int BUF_SIZE)
{
	do{
	if ((bytes = recv (sock, buf, BUF_SIZE-1, 0))==SOCKET_ERROR )
		{
			printf ("no data received: %d\n", WSAGetLastError ());
			return; 
		}
		else if ( bytes> 0)
		{
			buf[bytes]=0; // NULL terminate buffer
			recv_string += buf; //append to the string
		} 
	}while(bytes >0);
	//return recv_string;
}