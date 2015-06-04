/* winsock.cpp
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string>
#include <iostream>
#include <deque>
#include "wangj-headers.h"

using namespace std;


void winsock_test (deque<peer>& queue,set<peer,peer>& s)
{
	WSADATA wsaData;
	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2,2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError ());
		WSACleanup();	
		return ;
	}
	// string pointing to an HTTP server (DNS name or IP)
	char str [] = "gwc.gofoxy.net";
	
	// open a TCP socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == INVALID_SOCKET)
	{
		printf ("socket() generated error %d\n", WSAGetLastError ());
		WSACleanup ();	
		return;
	}

	// structure for connecting to server
	struct sockaddr_in server;

	// structure used in DNS lookups
	struct hostent *remote; 

	// first assume that the string is an IP address
	DWORD IP = inet_addr (str);
	if (IP == INADDR_NONE)
	{
		// if not a valid IP, then do a DNS lookup
		if ((remote = gethostbyname(str)) == NULL)
		{
			printf ("Invalid string: neither FQDN, nor IP address\n");
			return;
		}
		else // take the first IP address and copy into sin_addr
			memcpy((char *)&(server.sin_addr), remote->h_addr, remote->h_length);
	}
	else
	{
		// if a valid IP, directly drop its binary version into sin_addr
		server.sin_addr.S_un.S_addr = IP;
	}

	// setup the port # and protocol type
	server.sin_family = AF_INET;
	server.sin_port = htons (2108);		// host-to-network flips the byte order

	// connect to the server on port 80
	if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		printf ("Connection error: %d\n", WSAGetLastError ());
		return;
	}

	printf ("Successfully connected to %s (%s) on port %d\n", str, inet_ntoa (server.sin_addr), htons(server.sin_port));

	//-------------------------- Ready to send/receive -----------------
	const int BUF_SIZE = 512;
	char buf[BUF_SIZE];  // e.g.,BUF_SIZE = 200, prepare to receive data
	string recv_string = ""; // used to save all received data 
	string recv_string1 = "";
	int  bytes = 0;
	char sendstring [] = "GET /gwc/cgi-bin/fc?client=udayton0.1&hostfile=1 HTTP/1.0\nHost:gwc.gofoxy.net\n\n";  //use port 8080 for this host
	
	int bytesSent= send(sock, sendstring, sizeof(sendstring), 0); //send request
	if (bytesSent < 0)
		printf("Error sending request\n");
	else
	{
		do{
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
		}while ( bytes > 0 ); 
	}

	
	string temp("\n\r");
	size_t pos,pos1;

	//get the beginning position of the IP address
	pos = recv_string.find(temp);

	//get the rest of string
	string str1 = recv_string.substr(pos+3);

	
	string tep,tep1;
//	peer peers;
	//get the Ip address and poet and put them into the deque
	//set<peer,peer>::iterator ster;
	while(str1.size() != 0)
	{
		
		pos = str1.find(":");
		tep = str1.substr(0,pos);
		str1.erase(0,pos+1);
		pos1 = str1.find("\r");
		tep1 = str1.substr(0,pos1);
		str1.erase(0,pos1+2);
		peer peers; 
		peers.IP = inet_addr(tep.c_str());	
		peers.port = atoi(tep1.c_str());
		int size = s.size();
		s.insert(peers);
		//
		if(s.size() - size > 0)
			queue.push_back(peers);
	}	

	deque<peer>::iterator iter;
	iter = queue.begin();
	if(queue.size() != 0)
	{
		
		for(iter = queue.begin(); iter != queue.end();iter++)
		{
			struct in_addr temp;
			temp.S_un.S_addr = iter->IP;
			printf("%s:%d\n", inet_ntoa(temp), iter->port);
		}
	}

	
	// close the socket to this server; open again for the next one
	closesocket (sock);
	WSACleanup();

}

