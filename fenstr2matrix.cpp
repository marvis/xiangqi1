#include <iostream>
#include <string>
using namespace std;
void fenstr2matrix(string fenstr, string matrix[10][9])
{
	int i = 0, j = 0;
	for(string::iterator it = fenstr.begin(); it != fenstr.end(); it++)
	{
		char c = *it;
		if(c == 'r') matrix[j][i++] = "BC";
		else if(c == 'R') matrix[j][i++] = "RC";
		else if(c == 'n') matrix[j][i++] = "BM";
		else if(c == 'N') matrix[j][i++] = "RM";
		else if(c == 'b') matrix[j][i++] = "BX";
		else if(c == 'B') matrix[j][i++] = "RX";
		else if(c == 'a') matrix[j][i++] = "BS";
		else if(c == 'A') matrix[j][i++] = "RS";
		else if(c == 'k') matrix[j][i++] = "BJ";
		else if(c == 'K') matrix[j][i++] = "RJ";
		else if(c == 'c') matrix[j][i++] = "BP";
		else if(c == 'C') matrix[j][i++] = "RP";
		else if(c == 'p') matrix[j][i++] = "BZ";
		else if(c == 'P') matrix[j][i++] = "RZ";
		else if(c == '/') {j++; i = 0;}
		else if(c > '0' && c <= '9')
		{
			int count = c - '0';
			while(count--) matrix[j][i++] = "  ";
		}
	}
}

int main(int argc, char ** argv)
{
	string fenstr = argv[1];
	string matrix[10][9];
	fenstr2matrix(fenstr, matrix);
	for(int j = 0; j < 10; j++)
	{
		for(int i = 0; i < 9; i++)
		{
			cout<<matrix[j][i]<<" ";
		}
		cout<<endl;
	}
	return 0;
}
