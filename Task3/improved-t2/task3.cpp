#include <iostream>
#include <fstream>
#include <cstdio>
#include <queue>
#include <string>
#include <stack>
#include <ctime>

using namespace std;

//MAXN: node num; MAXLEN: len(a);

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
int f[MAXN][2];
int g[MAXN][MAXK];
short state[MAXN][MAXLEN];
int pre[MAXN][MAXLEN];

queue<int> Q[2];
bool inq[2][MAXN];
char ch[MAXN];
int edit[MAXK][MAXK];

int tf[MAXLEN][MAXN], pedit[MAXLEN][MAXN];
stack<string> tS;

ifstream fin("test2.in");
ofstream fout("test2.out");

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
	cout<<"Data loaded, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<"sec"<<endl;

}

void link(int u, int v) {
	if (str[u].substr(1, K - 1) != str[v].substr(0, K - 1)) return;

	node *p; p = new node;
	p->v = v; p->next = e[u]; e[u] = p;
}

void createGraph() {
	cout<<"Creating graph"<<endl;
	starttime = clock();

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			link(i, j);

	finishtime = clock();
	cout<<"graph created, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<"sec"<<endl;
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
		g[nodenum][i] = edit[K][i];
		pre[nodenum][i] = -1;
		state[nodenum][i] = -1;
	}
}

void pre_process() {
	cout<<"Pre_process: calculating distance f[i][K]"<<endl;
	starttime = clock();

	// set all elements to INF
	memset(f,0x3f,sizeof(f));
	for (int nodenum = 0; nodenum < N; nodenum++) {
		get_edit_distance(nodenum);
	}

	for (int i = 0; i< N; i++){
		inq[i][0] = false;
		inq[i][1] = false;
	}

	for (int _ = 0; _  < N; _++) {
		f[_][0] = g[_][0];
		Q[0].push(_);
		inq[0][_] = true;
		state[_][0] = -1;
		pre[_][0] = -1;
	}


	finishtime = clock();
	cout<<"Pre_process finished, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<"sec"<<endl;


}

void print_ops(string A, string B){
	// Task1

	int la, lb;
	la = A.length();
	lb = B.length();
	for (int i = 0; i <= la; i++)
		tf[i][0] = i;
	for (int j = 0; j <= lb; j++)
		tf[0][j] = j;

	/*
		pedit[i][j] description;
		0: from pedit[i - 1][j]
		1: from pedit[i][j - 1]
		2: from pedit[i - 1][j - 1] (a[i] == b[j])
		3: from pedit[i - 1][j - 1] (a[i] != b[j])
	 */

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

void print_result(int row) {

	int mindistance;
	int pos = -1;

	/*
		//cout<<A.length()<<endl;
				cout<<"Pre:"<<endl;
				for (int i = 0; i< N;i++)
				{
					for (int j = 0; j <=A.length(); j++)
						cout<<pre[i][j]<<" ";
					cout<<endl;
				}

				cout<<"State:"<<endl;
				for (int i = 0; i<N;i++)
				{
					for (int j = 0; j <= A.length(); j++)
						cout<<state[i][j]<<" ";
					cout<<endl;
				}

				cout<<"f:"<<endl;
				for (int i = 0; i<N;i++)
				{
					for (int j = 0; j <= 1; j++)
						cout<<f[i][j]<<" ";
					cout<<endl;
				}

				*/

		//return;


		mindistance = INF;
		//
		//
		row = (A.length() & 1) ^ 1;
		//cout<<row<<endl;

		for (int i = 0; i < N; i++)
			if (mindistance > f[i][row]) {
				mindistance = f[i][row];
				pos = i;
				//cout<<i<<"mini"<<" "<<endl;
			}
			//return;


			//cout<<"f[i][0 -- A.length]"<<endl;
			/*
				for (int i = 0; i < N; i++) {
					cout<<i<<" : "<<f[i][A.length()]<<endl;

					}
			*/


			//cout<<mindistance<<endl;

		cout<<"begin print"<<endl;


		int i,j;
		i = pos; j = A.length();
		string path_str,state_str, op_str;
		path_str = "";
		state_str = "";
		op_str="";

		while (state[i][j] >=0)
		{
			//cout<<i<<","<<j<<" "<<state[i][j]<<endl;
			state_str  = char(48 + state[i][j]) + state_str;

			if (state[i][j] != 4) path_str = ch[i] + path_str;

			//WARNING: 这边j可能有问题
			if (state[i][j] == 1)
			{
				//op_str = "INS "+ to_string(j) + " " + ch[i] + "\n" + op_str;
				i = pre[i][j];
				continue;
			}
			if (state[i][j] == 2 || state[i][j] == 3)
			{
				//if (state[i][j] == 3)
					//op_str = "SUB "+ to_string(j) + " " + ch[i] + "\n" + op_str;
				i = pre[i][j];
				j = j - 1;
				continue;
			}
			if (state[i][j] == 4)
			{
				//op_str = "DEL "+ to_string(j + 1) + "\n" + op_str;
				j = j - 1;
				continue;
			}
		}
		//cout<<"start:("<<i<<","<<j<<")"<<":"<<f[i][j]<<endl;



		path_str = str[i] + path_str;

		//cout<< path_str <<endl;
		//cout<< state_str <<endl;
		//cout<< op_str <<endl;
		//cout<< "dist:"<< mindistance<<endl;
		fout<<path_str<<endl;
		//cout<<mindistance<<endl;

		print_ops(A, path_str);

}

void work() {

	cout<<"Begin working"<<endl;
	starttime = clock();
	//i : node i; j: a[j]
	//for each node i ,calc edit distance with A
	//

	pre_process();




	int j = 1;
	for (int jj = 0; jj <= A.length(); jj++) {
		j = j ^ 1;
		int i;
		if (jj < K) {
			for (int _ = 0; _  < N; _++) {
				if (!inq[j][_]) {Q[j].push(_);inq[j][_] = true;}

				f[_][j ^ 1] = g[_][jj + 1];
				state[_][jj + 1] = -1;
				pre[_][jj + 1] = -1;

			}
		}
		while (!Q[j].empty())
		{
			//cout<<j<<" "<<i<<endl;
			i = Q[j].front();
			//考虑用f[i][j]来更新后面的f。i为结点编号，j为在A中位置
			node *p = e[i];
			//cout<<j<<","<<i<<endl;
			inq[j][i] = false;

			while (p!= NULL)
			{

				int k=p->v;
				//cout<<"e("<<i<<","<<k<<")"<<endl;

				if (f[k][j] > f[i][j] + 1)
				{
					f[k][j] = f[i][j] + 1;
					pre[k][jj] = i;
					state[k][jj] = 1;
					//cout<<k<<","<<jj<<","<<state[k][jj]<<endl;

					if (!inq[j][k])
					{
						inq[j][k] = true;
						Q[j].push(k);
					}
				}

				if (ch[k] != A[jj])
				{
					if (f[k][j ^ 1] > f[i][j] + 1)
					{
						f[k][j ^ 1] = f[i][j] + 1;
						pre[k][jj + 1] = i;
						state[k][jj + 1] = 2;
						//cout<<k<<","<<jj + 1<<","<<state[k][jj + 1]<<endl;



						if (!inq[j ^ 1][k])
						{
							inq[j ^ 1][k] = true;
							Q[j ^ 1].push(k);
						}
					}
				}
				else
				{
					if (f[k][j ^ 1] > f[i][j])
					{
						f[k][j ^ 1] = f[i][j];
						pre[k][jj + 1] = i;
						state[k][jj + 1] = 3;
						//cout<<k<<","<<jj + 1<<","<<state[k][jj + 1]<<endl;


						if (!inq[j ^ 1][k])
						{
							inq[j ^ 1][k] = true;
							Q[j ^ 1].push(k);
						}
					}
				}
				p=p->next;
			}

			if (f[i][j ^ 1] > f[i][j] + 1)
			{
				f[i][j ^ 1] = f[i][j] + 1;
				pre[i][jj + 1] = i;
				state[i][jj + 1] = 4;
				//cout<<i<<","<<jj<<","<<state[i][jj]<<endl;


				if (!inq[j ^ 1][i])
				{
					inq[j ^ 1][i] = true;
					Q[j ^ 1].push(i);
				}
			}

			Q[j].pop();
		}

		for (int _ = 0; _  < N; _++) {
			inq[j][_] = false;
			f[_][j] = INF;
		}

	}



	print_result(j);

	finishtime = clock();
	cout<<"Work finished, Time cost:"<< (double)(finishtime - starttime) / CLOCKS_PER_SEC <<"sec"<<endl;
}


int main() {


	init();
	createGraph();
	work();
	return 0;
}
