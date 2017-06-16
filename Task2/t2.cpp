#include <iostream>
#include <cstdio>
#include <queue>
using namespace std;

const int MAXN = 10010;
const int MAXMUM = 1000100;

string A;
int N, K;
string str[MAXN];
int f[MAXN][2];


struct node
{
	int v;
	node *next;
}*e[MAXN];
queue<int> Q[2];
bool inq[2][MAXN];



void init() {
	cin >> A;
	cin >> N;
	for (int i = 0; i < N; i++){
		cin>>str[i];
	}
	

	for (int i = 0; i < N; i++)
		e[i]=NULL;

	K = str[0].length();
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
	/*
		parameter:
			i: node i in graph
			j: A[1..j], length
			look from A[j]
		1. INS (you need to insert A[j])
		f[i,j] = f[i, j-1] + 1
		2. DEL (str(i-1) still work for matching A[j])
		f[i,j] = f[i-1, j] + 1
		3. SUB (if ch[i] != A[j])
		f[i,j] = f(i-1, j-1) + 1
		4. NO CHANGE if ch[i] == A[j])
		f[i,j] = f(i-1, j-1)

	 */

	//cout<<"AC!"<<endl;


	for (int i = 0; i < N; i++){
		f[i][0] = K;
		f[i][1] = MAXMUM;
		Q[0].push(i);
		inq[0][i] = true;
		inq[1][i] = false;
	}
	//cout<<K<<endl;

	int j = 0;
	for (int len = 0; len < A.length() - K; len++) {

		while (!Q[j].empty()) {
			int i = Q[j].front();
			inq[j][i] = false;
			//cout<<i<<endl;

			// State 1. INS
			if (f[i][1 - j] > f[i][j] + 1) {
				f[i][1 - j] = f[i][j] + 1;
				if (!inq[1 - j][i]) {
						Q[1 - j].push(i);
						inq[1 - j][i] = true;
				}
			}

			node *p = e[i];

			while (p!=NULL) {
				int k = p->v;
				// State 2. DEL
				if (f[k][j] > f[i][j] + 1) {
					f[k][j] = f[i][j] + 1;
					if (!inq[j][k])
						{
							Q[j].push(k);
							inq[j][k] = true;
						}

				}

				//State 3. SUB
				if (A[len] != str[k][K - 1]) {  //????
					if (f[k][1 - j] > f[i][j] + 1) {
						f[k][1 - j] = f[i][j] + 1;
						if (!inq[1-j][k]) {
							Q[1-j].push(k);
							inq[1-j][k] = true;
						}
					}

				}
				else {
				//State 4: NO CHANGE
					if (f[k][1 - j] > f[i][j]) {
						f[k][1 - j] = f[i][j];
						if (!inq[1-j][k]) {
							Q[1-j].push(k);
							inq[1-j][k] = true;
						}
					}
				}

				p = p->next;
			}


			Q[j].pop();
			if (!inq[i]) {
				f[j][i] = MAXMUM;
			}

		}
		j = 1 - j;
	}

	int mindistance = K + A.length();

	for (int i = 0; i < N; i++)
		cout<<i<<":"<<f[i][j]<<endl;

	for (int i = 0; i < N; i++)
		if (f[i][j] < mindistance){
			mindistance = f[i][j];
		}


	cout<<mindistance<<endl;
}

int main() {
	freopen("task2.in", "r", stdin);
	//freopen("a.out", "w", stdout);

	init();
	createGraph();
	work();
	return 0;
}