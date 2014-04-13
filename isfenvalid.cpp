#include <iostream>
#include <string>
#include <vector>
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
bool isfenvalid(string prev_fenstr, string cur_fenstr)
{
	if(prev_fenstr == "") return true;
	string prev_matrix[10][9];
	string cur_matrix[10][9];
	fenstr2matrix(prev_fenstr, prev_matrix);
	fenstr2matrix(cur_fenstr, cur_matrix);
	vector<int> diffposes;
	for(int j = 0; j < 10; j++)
	{
		for(int i = 0; i < 9; i++)
		{
			if(cur_matrix[j][i] != prev_matrix[j][i]) diffposes.push_back(j*9+i);
		}
	}
	if(diffposes.size() == 0 || diffposes.size() == 2) return true;
	return false;
}

int main(int argc, char ** argv)
{
	if(argc == 2)
	{
		cout<<"true"<<endl;
		return 0;
	}
	if(argc == 1 || argc > 3) 
	{
		cout<<"false"<<endl;
		return 0;
	}
	string prev_fenstr = argv[1];
	string cur_fenstr = argv[2];
	if(isfenvalid(prev_fenstr, cur_fenstr))
	{
		cout<<"true"<<endl;
	}
	return 0;
}
