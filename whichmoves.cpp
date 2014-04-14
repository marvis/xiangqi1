#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <vector>
using namespace std;

string blacklines[9] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
string redlines[9] = {"一", "二", "三", "四", "五", "六", "七", "八", "九"};
void fenstr2matrix(string fenstr, string matrix[10][9])
{
	int i = 0, j = 0;
	for(string::iterator it = fenstr.begin(); it != fenstr.end(); it++)
	{
		char c = *it;
		if(c == 'r') matrix[j][i++] = "黑车";
		else if(c == 'R') matrix[j][i++] = "红车";
		else if(c == 'n') matrix[j][i++] = "黑马";
		else if(c == 'N') matrix[j][i++] = "红马";
		else if(c == 'b') matrix[j][i++] = "黑象";
		else if(c == 'B') matrix[j][i++] = "红相";
		else if(c == 'a') matrix[j][i++] = "黑士";
		else if(c == 'A') matrix[j][i++] = "红仕";
		else if(c == 'k') matrix[j][i++] = "黑将";
		else if(c == 'K') matrix[j][i++] = "红帅";
		else if(c == 'c') matrix[j][i++] = "黑炮";
		else if(c == 'C') matrix[j][i++] = "红炮";
		else if(c == 'p') matrix[j][i++] = "黑卒";
		else if(c == 'P') matrix[j][i++] = "红兵";
		else if(c == '/') {j++; i = 0;}
		else if(c > '0' && c <= '9')
		{
			int count = c - '0';
			while(count--) matrix[j][i++] = "  ";
		}
	}
}

string whichmoves(string from_fenstr, string to_fenstr)
{
	assert(from_fenstr != "");
	
	int linenum = 0;
	bool isopposite = false;
	for(int i = 0; i < from_fenstr.size(); i++) 
	{
		if(from_fenstr[i] == '/') linenum++;
		if(from_fenstr[i] == 'k') break;
	}
	if(linenum > 2) isopposite = true;

	string from_matrix[10][9];
	string to_matrix[10][9];
	fenstr2matrix(from_fenstr, from_matrix);
	fenstr2matrix(to_fenstr, to_matrix);
	vector<int> diffposes;
	for(int j = 0; j < 10; j++)
	{
		for(int i = 0; i < 9; i++)
		{
			if(to_matrix[j][i] != from_matrix[j][i]) diffposes.push_back(j*9+i);
		}
	}
	if(diffposes.size() != 2) return "";
	if(diffposes.size() == 2)
	{
		int pos1 = diffposes[0];
		int pos2 = diffposes[1];
		int i1 = pos1 % 9, j1 = pos1 / 9;
		int i2 = pos2 % 9, j2 = pos2 / 9;
		if(to_matrix[j1][i1] != "  " && to_matrix[j2][i2] != "  ") return "";
		if(to_matrix[j1][i1] == "  " && to_matrix[j2][i2] == "  ") return "";
		if(to_matrix[j1][i1] != "  " && from_matrix[j2][i2] != to_matrix[j1][i1]) return "";
		if(to_matrix[j2][i2] != "  " && from_matrix[j1][i1] != to_matrix[j2][i2]) return "";

		string movechess;
		int fromi, fromj, toi, toj;
		if(to_matrix[j1][i1] != "  ")
		{
			movechess = to_matrix[j1][i1];
			fromi = i2; fromj = j2;
			toi = i1; toj = j1;
		}
		else 
		{
			movechess = to_matrix[j2][i2];
			fromi = i1; fromj = j1;
			toi = i2; toj = j2;
		}
		ostringstream oss;
		oss<<fromj+1<<" "<<fromi+1<<" "<<toj+1<<" "<<toi+1;
		return oss.str();
	}
	return "";
}
int main(int argc, char ** argv)
{
	if(argc != 3) cout<<""<<endl;
	else 
		cout<< whichmoves(argv[1], argv[2])<<endl;
}
