#include <iostream>
#include <cstdio>
#include <queue>
using namespace std;

//MAXN: node num; MAXLEN: len(a);

const int MAXN = 100;
const int INF = 1000100;
const int MAXLEN = 100;

struct node
{
	int v;
	node *next;
}*e[MAXN];



string A;
int N, K;
string str[MAXN];
int f[MAXN][MAXLEN];
int state[MAXN][MAXLEN], pre[MAXN][MAXLEN];

queue<int> Q[2];
bool inq[2][MAXN];
char ch[MAXN];


void init() {
	cin >> A;
	cin >> N;
	for (int i = 0; i < N; i++)
		cin>>str[i];
	K = str[0].length();

	for (int i = 0; i < N; i++)
		ch[i]= str[i][K - 1];

	for (int i = 0; i < N; i++)
		e[i]=NULL;


}
void link(int u, int v) {
	//substr: pos + len
	//cout<<str[u].substr(1, K - 1)<<", "<<str[v].substr(0, K - 1)<<endl;

	if (str[u].substr(1, K - 1) != str[v].substr(0, K - 1)) return;
	//cout<<u<<","<<v<<endl;
	node *p;
	p = new node;
	p->v = v;
	p->next = e[u];
	e[u] = p;
}
void createGraph() {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			link(i, j);
}
void work() {
	//i : node i; j: a[j]
	memset(inq, 0, sizeof(inq));

	for (int i = 0; i < N; i++){
		f[i][0] = K;
		pre[i][0] = -1;
		state[i][0] = -1;
		Q[0].push(i);
		inq[0][i] = true;
	}

	for (int j = 0; j < A.length() - 1; j++) {
		for (int i = 0; i < N; i++)
			f[i][j + 1] = INF;
		int i;
		while (!Q[j].empty())
		{
			i = Q[j].front();
			node *p = e[i];
			while (p!= NULL)
			{
				int k=p->v;

				if (f[k][j] > f[i][j] + 1)
				{
					f[k][j] = f[i][j] + 1;
					pre[k][j] = i;
					state[k][j] = 1;

					if (!inq[j][k])
					{
						inq[j][k] = true;
						Q[j].push(k);
					}
				}
				if (ch[k] != A[j + 1])
				{
					if (f[k][j + 1] > f[i][j] + 1)
					{
						f[k][j + 1] = f[i][j] + 1;
						pre[k][j + 1] = i;
						state[k][j + 1] = 2;

						if (!inq[j+1][k])
						{
							inq[j+1][k] = true;
							Q[j+1].push(k);
						}
					}
				}
				else
				{
					if (f[k][j + 1] > f[i][j])
					{
						f[k][j + 1] = f[i][j];
						pre[k][j + 1] = i;
						state[k][j + 1] = 3;

						if (!inq[j+1][k])
						{
							inq[j+1][k] = true;
							Q[j+1].push(k);
						}
					}
				}

				if (f[i][j + 1] > f[i][j] + 1)
				{
					f[i][j + 1] = f[i][j] + 1;
					pre[i][j + 1] = i;
					state[i][j + 1] = 4;

					if (!inq[j+1][i])
					{
						inq[j+1][i] = true;
						Q[j+1].push(i);
					}
				}

				p=p->next;
			}
			Q[j].pop();
		}
	}

	int mindistance = INF;
	int pos = -1;
	for (int i = 0; i < N; i++)
		if (mindistance > f[A.length() - 1][i]) {
			mindistance = f[A.length() - 1][i];
			pos = i;
		}
	int i,j;
	i = pos; j = A.length() - 1;
	string path_str;
	path_str = "";
	while (pre[i][j] >=0)
	{
		path_str = ch[i] + path_str;

		if (state[i][j] == 1) {i = pre[i][j]; continue;}
		if (state[i][j] == 2 || state[i][j] == 3) {i = pre[i][j]; j = j - 1; continue;}
		if (state[i][j] == 4) {j = j - 1; continue;}
	}
	path_str = str[i].substr(0, K - 1) + path_str;

	cout<< path_str <<endl;
	cout<< mindistance<<endl;
	//TODO: print the operations(use task1 method)

}

int main() {
	freopen("a.in", "r", stdin);
	//freopen("a.out", "w", stdout);

	init();
	createGraph();
	work();
	return 0;
}
