#include <iostream>
#include <cstdio>

using namespace std;

int N;
string str, src, trg;
int p1;
char ch;
int main() {

	//ofstream fin()
	freopen("task1.in", "r", stdin);
	cin>>src>>trg;
	fclose(stdin);


	freopen("task1.out", "r", stdin);
	
	cin>>N;
	while (N--){
		cin>>str;
		if (str[0] == 'D') {
			cin>>p1;
			src.erase(p1, 1);
		}
		if (str[0] == 'I') {
			cin>>p1>>ch;
			src.insert(p1, 1, ch);
		}
		if (str[0] == 'S') {
			cin>>p1>>ch;
			src[p1] = ch;
		}
	}

	freopen("result.out", "w", stdout);
	if (src == trg) 
		cout<<"Same!"<<endl;
	else 
	{
		cout<<"Different"<<endl;
		cout<<src<<endl;
		cout<<trg<<endl;
	}


}