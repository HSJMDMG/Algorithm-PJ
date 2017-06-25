#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <stack>
#include <vector>
#include <sstream>



using namespace std;

const int MAXN = 10010;

string A,B;
int la, lb;
int f[MAXN][MAXN];
int edit[MAXN][MAXN];
stack<string> S;


string ToStr(int i) {
	stringstream ss;
	ss<<i;
	return ss.str();
}


int main() {


	freopen("input.in","r", stdin);
	freopen("output.out","w", stdout);
	//freopen("task1.in","r", stdin);
	//freopen("task1.out","w", stdout);

	cin>>A;
	cin>>B;
	la = A.length();
	lb = B.length();

	//cout<<la<<endl;
	//cout<<lb<<endl;
	for (int i = 0; i <= la; i++)
		f[i][0] = i;
	for (int j = 0; j <= lb; j++)
		f[0][j]=j;

	/*
		edit[i][j] description;
		0: from edit[i - 1][j]
		1: from edit[i][j - 1]
		2: from edit[i - 1][j - 1] (a[i] == b[j])
		3: from edit[i - 1][j - 1] (a[i] != b[j])
	 */

	for (int i = 1; i <= la; i++)
		for (int j = 1; j <= lb; j++)
		{
			edit[i][j] = 0;
			f[i][j] = f[i - 1][j] + 1;


			if (f[i][j - 1] + 1 < f[i][j])
			{
				f[i][j] = f[i][j - 1] + 1;
				edit[i][j] = 1;
			}

			if (A[i - 1] == B[j - 1])
			{
				if (f[i - 1][j - 1] < f[i][j])
				{
					f[i][j] = f[i - 1][j - 1];
					edit[i][j] = 2;
				}
			}
			else
			{
				if (f[i - 1][j - 1] + 1 < f[i][j])
				{
					f[i][j] = f[i - 1][j - 1] + 1;
					edit[i][j] = 3;
				}	
			}
		}



		cout<<f[la][lb]<<endl;
		int i, j;
		i = la; j = lb;

	/*
		edit[i][j] description;
		0: from edit[i - 1][j] delete i
		1: from edit[i][j - 1] increas i
		2: from edit[i - 1][j - 1] (a[i] == b[j])  //no operation
		3: from edit[i - 1][j - 1] (a[i] != b[j])  // replace i with j
	 */

	
	//S.clear();	

		while (i > 0 || j > 0) {

			string str = "";

			if (edit[i][j] == 0) {
			//	str  = "DEL " + ToStr((j));
				str = "DEL " + ToStr(i - 1);
				S.push(str);
				i--;
				continue;
			}
			if (edit[i][j] == 1) {
			//	S.push("INS " + ToStr(j - 1)  + " " + B[j - 1]);
				S.push("INS " + ToStr(i)  + " " + B[j - 1]);
				j--;
			}
			if (edit[i][j] == 2) {
				i--;
				j--;
			} 
			if (edit[i][j] == 3) {
			//	S.push("SUB " + ToStr((j - 1))+ " " + B[j - 1]);
				S.push("SUB " + ToStr((i - 1))+ " " + B[j - 1]);
				i--;
				j--;
			}
		}

		while (i > 0)
		{
			S.push("DEL " + ToStr(i - 1));
			//cout<<"Rest"<<endl;
			i--;
		}

		while (j > 0)
		{
			//S.push("INS " + ToStr(j - 1) + " " + B[j - 1]);
			S.push("INS " + ToStr(0) + " " + B[j - 1]);
			j--;
		}



	while (!S.empty()) {
		cout<<S.top()<<endl;
		S.pop();
	}



	return 0;
}