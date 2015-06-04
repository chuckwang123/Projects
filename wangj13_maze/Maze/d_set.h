#include<vector>
#include"maze_cell.h"
using namespace std;

class D_set
{
public:
	D_set();
	D_set(int numElements);
	void D_set1(int &numElements);
	int find(int x);
	void unionSets(int root1, int root2);
	void setwall(int x);
	void setempty(int x);
	int getsize();
	void display();
	int& operator[] (const int x);
private:
	vector<int>maze_data;
	int size;
};