/* main.cpp
 */
#include "wangj-headers.h"


using namespace std;


// function inside winsock.cpp
void winsock_test (deque<peer>&,set<peer,peer>&);

bool Receive(SOCKET, string &recv_string ); 

int CrawlPeer(deque<peer>& ,deque<peer>&, DWORD peer_IP, unsigned short peer_port);

string  dnslookup( DWORD);

void recvnew(string &recv_string, int bytes, SOCKET sock, char buf[], int BUF_SIZE);
// this function is where the thread starts
UINT thread(LPVOID pParam)
{
	deque<peer> queue,lqueue;
	Parameters *p = ((Parameters*)pParam);
	peer temp;
	vector<peer> temp_vec;
	int comp;

	HANDLE arr[] = {p->eventQuit, p->semaQ};
	peer Q;
	
	while (true)
	{ 
		if(WaitForMultipleObjects(2, arr, false, INFINITE) == WAIT_OBJECT_0 + 1 )
		{ 
			WaitForSingleObject(p->mutex,INFINITE);
			
			p->active_threads ++; 
			if(p->queue.size() == 0)
				return 0;
			Q = p->queue.front();
			p->queue.pop_front();
			temp_vec.clear();

			temp_vec.push_back(Q);
			queue.clear();
			lqueue.clear();

			ReleaseMutex(p->mutex);	// release critical section

			comp = CrawlPeer(queue,lqueue,Q.IP, Q.port);
			
			WaitForSingleObject(p->mutex,INFINITE); 

			if(comp == 1)
			{
				p->count++;
			}
			while(!lqueue.empty())  
			{
				temp = lqueue.front();
				lqueue.pop_front();
				p->leave.insert(temp);
				temp_vec.push_back(temp);
			}
			 
			while(queue.size()!= 0)
			{
				temp = queue.front();
				temp_vec.push_back(temp);
				queue.pop_front();
				int size = p->set.size();
				p->set.insert(temp);
				if(p->set.size()-size > 0)
				{
					p->queue.push_back(temp);
					ReleaseSemaphore(p->semaQ, 1, NULL);
				}
			}	
			 
			p->active_threads--;   // finished crawling one peer
			p->times++;
			p->final_queue.insert(pair<int,vector<peer>>(p->times,temp_vec));
			if ((p->queue.size() == 0 && p->active_threads == 0))   // the last thread set quit event
			{
				SetEvent(p->eventQuit);
			}
			ReleaseMutex(p->mutex);                  
		}
		else
		{
			return 0;
		}
	}

}

int main(void)
{
	int i =0;
	ofstream out,out_peer;
	out.open("1.txt");
	out_peer.open("2.txt");
	printf ("-----------------\n");
	WSADATA wsaData;
	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2,2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError ());
		WSACleanup();	
		return 0;
	}


	// thread handles are stored here; they can be used to check status of threads, or kill them
	HANDLE *ptrs = new HANDLE [1000];

	Parameters p;
	
	winsock_test(p.queue,p.set);

	// create a mutex for accessing critical sections (including printf)
	p.mutex = CreateMutex (NULL, 0, NULL);
	
	// create a semaphore that counts the number of active threads
	p.semaQ = CreateSemaphore(NULL, 0, 10000, NULL);
	p.eventQuit = CreateEvent (NULL, true, false, NULL);
	p.active_threads =0;
	p.count = 0;
	p.times = 0;

	ReleaseSemaphore(p.semaQ, p.queue.size(), NULL);

	// get current time
	DWORD t = timeGetTime();

	printf("-----------created thread-----\n");	
	// structure p is the shared space between the threads
	for(int i = 0;i < 1000;i++)
	{	
		ptrs[i] = CreateThread(NULL, 4096, (LPTHREAD_START_ROUTINE)thread, &p, 0, NULL);
	}
	
	WaitForSingleObject(p.eventQuit, INFINITE);
	
	out << "the total number of ultrapeers is " << p.set.size() << endl;

	out << "the number of ultrapeers' response is " << p.count << endl;

	out << "the number of leaves is " << p.leave.size() << endl;

	out << "the time for crawling is " << timeGetTime()-t << "ms" <<endl;

	out << "the average rate of crawling is " << (double)p.count /(timeGetTime()-t)*1000 << endl;

	map<int,vector<peer>> ::iterator iter;
	for(iter = p.final_queue.begin(); iter != p.final_queue.end(); iter++)
	{
		out_peer << iter->first <<" out-degree is " << iter->second.size() << "  ";
		for(int i = 0;i < iter->second.size(); i++)
		{
			out_peer << iter->second[i].IP << "  ";
		}
		out_peer << endl;
	}
	
	delete[] ptrs; 
	ptrs = NULL;
	WSACleanup();
	out.close();
	return 0; 
}

