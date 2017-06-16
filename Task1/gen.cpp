#include <cstdio>
#include <iostream>
#include <ctime>
#include <fstream>


using namespace std;

ofstream fout("input.in");
string stra, strb;


int main() {


	srand(time(0));
	int la,lb;
	char ch;

	//la = rand() % 10000 + 1; lb = rand() % 10000 + 1;
	//la = 10000; lb = 10000;
	la = rand() % 10; lb = rand() % 10;
	stra = "" ; strb = "";
	for (int i = 0; i < la; i++)
	{
		ch = 65 + rand() % 26;
		stra += ch;
	}

	for (int i = 0; i < lb; i++)
	{
		ch = 65 + rand() % 26;
		strb += ch;
	}

	cout<<la<<", " << lb<<endl;
	cout<<stra<<endl<<strb<<endl;
	fout<<stra<<endl;
	fout<<strb<<endl;

	return 0;
}