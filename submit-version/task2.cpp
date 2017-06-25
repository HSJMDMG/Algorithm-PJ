#include <iostream>
#include <fstream>
#include <cstdio>
#include <queue>
#include <string>
#include <stack>
#include <ctime>

using namespace std;

clock_t starttime, finishtime;

const int MAXN = 10010;
const int INF = 1000100;
const int MAXLEN = 10010;
const int MAXK = 35;

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

queue<int> Q[MAXLEN];
bool inq[MAXLEN][MAXN];
char ch[MAXN];
int edit[MAXK][MAXK];

int tf[MAXLEN][MAXN], pedit[MAXLEN][MAXN];
stack<string> tS;

ifstream fin("task2.in");
ofstream fout("task2.out");

int min(int a, int b){
	return a<b?a:b;
}

void init() {

	cout<<"Loading Data"<<endl;
	starttime = clock();

	fin >> A;
	fin >> N;
	for (int i = 0; i < N; i++)
		fin>>str[i];
	K = str[0].length();

	for (int i = 0; i < N; i++)
		ch[i]= str[i][K - 1];

	for (int i = 0; i < N; i++)
		e[i]=NULL;

	finishtime = clock();
	cout<<"Data loaded, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<endl;

}

void link(int u, int v) {
	if (str[u].substr(1, K - 1) != str[v].substr(0, K - 1)) return;
	node *p;
	p = new node;
	p->v = v;
	p->next = e[u];
	e[u] = p;
}

void createGraph() {
	cout<<"Creating graph"<<endl;
	starttime = clock();

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			link(i, j);

	finishtime = clock();
	cout<<"graph created, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<endl;
}

void get_edit_distance(int nodenum) {
	for (int i = 0; i <= K; i++){
		edit[i][0] = i;
		edit[0][i] = i;
	}
	for (int i = 1; i <= K; i++)
		for (int j = 1; j <= K; j++)
		{
			edit[i][j] = edit[i - 1][j] + 1;
			edit[i][j] = min(edit[i][j], edit[i][j - 1] + 1);
			if (str[nodenum][i - 1] == A[j - 1])
				edit[i][j] = min(edit[i][j], edit[i - 1][j - 1]);
			else
				edit[i][j] = min(edit[i][j], edit[i - 1][j - 1] + 1);
		}

	for (int i = 0; i <= K; i++) {
		f[nodenum][i] = edit[K][i];
		pre[nodenum][i] = -1;
		state[nodenum][i] = -1;
	}
}

void pre_process() {

	cout<<"Pre_process: calculating distance f[i][K]"<<endl;
	starttime = clock();



	memset(f,0x3f,sizeof(f));
	for (int nodenum = 0; nodenum < N; nodenum++) {
		get_edit_distance(nodenum);
	}


	finishtime = clock();
	cout<<"Pre_process finished, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<endl;

}

void print_ops(string A, string B){

	int la, lb;
	la = A.length();
	lb = B.length();

	for (int i = 0; i <= la; i++)
		tf[i][0] = i;
	for (int j = 0; j <= lb; j++)
		tf[0][j]=j;

	for (int i = 1; i <= la; i++)
		for (int j = 1; j <= lb; j++)
		{
			pedit[i][j] = 0;
			tf[i][j] = tf[i - 1][j] + 1;


			if (tf[i][j - 1] + 1 < tf[i][j])
			{
				tf[i][j] = tf[i][j - 1] + 1;
				pedit[i][j] = 1;
			}

			if (A[i - 1] == B[j - 1])
			{
				if (tf[i - 1][j - 1] < tf[i][j])
				{
					tf[i][j] = tf[i - 1][j - 1];
					pedit[i][j] = 2;
				}
			}
			else
			{
				if (tf[i - 1][j - 1] + 1 < tf[i][j])
				{
					tf[i][j] = tf[i - 1][j - 1] + 1;
					pedit[i][j] = 3;
				}
			}
		}

		int i, j;
		i = la; j = lb;
		fout<<tf[i][j]<<endl;
		while (i > 0 || j > 0) {
			string str = "";
			if (pedit[i][j] == 0) {
				str = "DEL " + to_string(i - 1);
				tS.push(str);
				i--;
				continue;
			}
			if (pedit[i][j] == 1) {
				tS.push("INS " + to_string(i)  + " " + B[j - 1]);
				j--;
			}
			if (pedit[i][j] == 2) {
				i--;
				j--;
			}
			if (pedit[i][j] == 3) {
				tS.push("SUB " + to_string((i - 1))+ " " + B[j - 1]);
				i--;
				j--;
			}
		}

		while (i > 0)
		{
			tS.push("DEL " + to_string(i - 1));
			i--;
		}
		while (j > 0)
		{
			tS.push("INS " + to_string(0) + " " + B[j - 1]);
			j--;
		}



	while (!tS.empty()) {
		fout<<tS.top()<<endl;
		tS.pop();
	}

}

void print_result() {

	int mindistance;
	int pos = -1;
		mindistance = INF;
		for (int i = 0; i < N; i++)
			if (mindistance > f[i][A.length()]) {
				mindistance = f[i][A.length()];
				pos = i;
			}

		cout<<"begin print"<<endl;


		int i,j;
		i = pos; j = A.length();
		string path_str,state_str, op_str;
		path_str = "";
		state_str = "";
		op_str="";

		while (pre[i][j] >=0)
		{
			state_str  = char(48 + state[i][j]) + state_str;
			if (state[i][j] != 4) path_str = ch[i] + path_str;

			if (state[i][j] == 1)
			{
				op_str = "INS "+ to_string(j) + " " + ch[i] + "\n" + op_str;
				i = pre[i][j];
				continue;
			}
			if (state[i][j] == 2 || state[i][j] == 3)
			{
				if (state[i][j] == 3)
					op_str = "SUB "+ to_string(j) + " " + ch[i] + "\n" + op_str;
				i = pre[i][j];
				j = j - 1;
				continue;
			}
			if (state[i][j] == 4)
			{
				op_str = "DEL "+ to_string(j + 1) + "\n" + op_str;
				j = j - 1;
				continue;
			}
		}



		path_str = str[i] + path_str;

		fout<<path_str<<endl;

		print_ops(A, path_str);

}

void work() {

	cout<<"Begin working"<<endl;
	starttime = clock();

	pre_process();

	for (int i = 0; i<= A.length(); i++)
		for (int j = 0; j< N; j++)
			inq[i][j] = 0;

	for (int j = 0; j <= A.length(); j++) {

		int i;
		if (j<=K) {
			for (int _ = 0; _  < N; _++) {
				if (!inq[j][_]) {Q[j].push(_);inq[j][_] = true;}
			}
		}
		while (!Q[j].empty())
		{
			i = Q[j].front();
			//考虑用f[i][j]来更新后面的f。i为结点编号，j为在A中位置
			node *p = e[i];
			inq[j][i] = false;

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

				if (ch[k] != A[j])
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

				p=p->next;
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

			Q[j].pop();

		}

	}

	print_result();


	finishtime = clock();
	cout<<"Work finished, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<endl;
}


int main() {
	init();
	createGraph();
	work();
	return 0;
}
