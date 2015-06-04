#include"wangj-headers.h"
#include <fstream>

bool Receive(SOCKET sock, string &recv_string,int bytes, char buf[], int BUF_SIZE );

int CrawlPeer(deque<peer>& peerqueue,deque<peer>& leavequeue, DWORD peer_IP, unsigned short peer_port) 
{

	WSADATA wsaData;
	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2,2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError ());
		WSACleanup();	
		return 0;
	}
	 
	const int BUF_SIZE = 512;
	char buff[BUF_SIZE];
	SOCKET peersocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(peersocket == INVALID_SOCKET)
	{
		printf ("socket() generated error %d\n", WSAGetLastError ());
		WSACleanup ();	
		return 0;
	}
	struct sockaddr_in server;

	// structure used in DNS lookups
	struct hostent *remote; 
	DWORD IP =peer_IP;

	struct in_addr temp11;
	temp11.S_un.S_addr = peer_IP;

	if (IP == INADDR_NONE)
	{
		// if not a valid IP, then do a DNS lookup
		if ((remote = gethostbyname(inet_ntoa(temp11)))== NULL)
		{	
			printf ("Invalid string: neither FQDN, nor IP address\n");
			return 0;
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
	server.sin_port =  htons (peer_port);		// host-to-network flips the byte order

	
	// connect to the server on port 80
	if (connect(peersocket, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		printf ("Connection error: %d\n", WSAGetLastError ());
		return 0;
	}
	
	printf ("Successfully connected to %s (%s) on port %d\n", inet_ntoa(temp11), inet_ntoa (server.sin_addr), htons(server.sin_port));

	char string1[]= "GNUTELLA CONNECT/0.6\r\nUser-Agent:udayton_cs_crawler/1.0\r\nX-Ultrapeer: False\r\nCrawler:0.1\r\n\r\n";

	int bytesSent1= send(peersocket, string1, sizeof(string1), 0); //send requestcout 
	
	int bytes = 0;
	bool result;

	string recv_string1 = "";
	
	if (bytesSent1 < 0)
		printf("Error sending request\n");
	else
	{
		result = Receive(peersocket, recv_string1,bytes, buff, BUF_SIZE );
	}
	 
	if(result == false || recv_string1.size()==0)
	{
		return 0;
	}
	 
	string temp("Peers: ");
	string temp1("\nLeaves: ");
	size_t pos,pos1,pos2;
	string str1;
	//get the beginning position of the IP address
	 
	pos = recv_string1.find(temp);
 
	pos2 = recv_string1.find(temp1);

	str1 = recv_string1.substr(pos+7,pos2-pos-8);
	
	string str2 = recv_string1.substr(pos2 +9);
	str1 +=",";

	str2 += ",";
	string tep,tep1;
	//get the Ip address and poet and put them into the deque
	 
	while(str1.size() != 0)
	{
		peer peers;
		pos = str1.find(":");
		tep = str1.substr(0,pos);
		peers.IP = inet_addr(tep.c_str());
		str1.erase(0,pos+1);

		pos1 = str1.find(",");
		tep1 = str1.substr(0,pos1);
		str1.erase(0,pos1+1);
		peers.port = atoi(tep1.c_str());
	
		peerqueue.push_back(peers);
	}
	
	
	while(str2.size() != 0)
	{
		peer peers;
		pos = str2.find(":");
		tep = str2.substr(0,pos);
		peers.IP = inet_addr(tep.c_str());
		str2.erase(0,pos+1);

		pos1 = str2.find(",");
		tep1 = str2.substr(0,pos1);
		str2.erase(0,pos1+1);
		peers.port = atoi(tep1.c_str());

		leavequeue.push_back(peers);
	}
	
	closesocket (peersocket);
	WSACleanup();
	return 1;
}