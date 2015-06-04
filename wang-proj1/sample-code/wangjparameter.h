
#pragma pack(push)
#pragma pack(1)
class peer
{
public:
	DWORD IP;
	unsigned short port;
	bool operator ()(peer x,peer y)
	{
		return memcmp(&x,&y,sizeof(peer)) < 0;
	}
};
#pragma pack(pop)

#include <deque>
#include "wangj-headers.h"
using namespace std;


class Parameters
{
public:
	HANDLE mutex;
	HANDLE eventQuit;
	HANDLE semaQ;
	int count;
	int times;
	int active_threads;
	deque<peer> queue;
	map<int, vector<peer> > final_queue;
	set<peer, peer> set,leave;
};
