#include <ws2tcpip.h>
#include "wangj-headers.h"
#include <iostream> 

using namespace std;

string dnslookup( DWORD IP )
{
	string name="";  // empty name
	DWORD dwRetval;
	struct sockaddr_in saGNI; // containing the add & port # of the socket
	char hostname[NI_MAXHOST]; //NI_MAXSERV=32, defined in <ws2tcpip.h>
	char servInfo[NI_MAXSERV]; //NI_MAXHOST=1025
	     // port number
	saGNI.sin_family = AF_INET;


		u_short port = 27015; 
		saGNI.sin_addr.s_addr = IP; //if use string, inet_addr(strIP.c_str());
		saGNI.sin_port = htons(port);
		dwRetval = getnameinfo( (struct sockaddr *) &saGNI,sizeof (struct sockaddr),hostname,NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV );

		//Sleep(100);

		if (dwRetval != 0) 
		{
			printf("getnameinfo failed with error # %ld\n",WSAGetLastError()); 
		}
		else 
		{
			name = hostname;
		}
	return name;
}