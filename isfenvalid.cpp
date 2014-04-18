#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <cstdio>
#include <cstdlib>
using namespace std;
#define ABS(x) ((x) >= 0 ? (x) : -(x))
bool verbose = 0;
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

bool isfenvalid(string fenstr)
{
	string matrix[10][9];
	string chessnames[14] = {"BC", "BM", "BX", "BS", "BJ", "BP", "BZ", 
	"RC", "RM", "RX", "RS", "RJ", "RP", "RZ"};

	int linenum = 0;
	bool isreverse = false;
	for(int i = 0; i < fenstr.size(); i++) 
	{
		if(fenstr[i] == '/') linenum++;
		if(fenstr[i] == 'k') break;
	}
	if(linenum > 2) 
	{
		isreverse = true;
		for(int i = 0; i < fenstr.size()/2; i++)
		{
			char c = fenstr[i];
			fenstr[i] = fenstr[fenstr.size() - 1 - i];
			fenstr[fenstr.size() - 1 - i] = c;
		}
		//reverse(fenstr.begin(), fenstr.end()); // cann't compile to android
	}

	fenstr2matrix(fenstr, matrix);
	// 1. check the chess num
	map<string, int> chessNum;
	for(int id = 0; id < 14; id++) chessNum[chessnames[id]] = 0;
	for(int j = 0; j < 10; j++)
	{
		for(int i = 0; i < 9; i++)
		{
			int ti = i, tj = j; // tj is used for output
			if(isreverse) 
			{
				ti = 8-i;
				tj = 9-j;
			}

			string chess = matrix[j][i];
			chessNum[chess]++;
			if(chess == "BJ")
			{
				int jj = j;
				if(jj > 2 || i < 3 || i > 5) 
				{
					if(verbose) cerr<<"invalid BJ position at ("<<tj<<","<<ti<<")"<<endl;
					return false;
				}
			}
			if(chess == "RJ")
			{
				int jj = 9-j;
				if(jj > 2 || i < 3 || i > 5) 
				{
					if(verbose) cerr<<"invalid RJ position at ("<<tj<<","<<ti<<")"<<endl;
					return false;
				}
			}
			if(chess == "BX")
			{
				int jj = j;
				if(!(((jj==0 || jj == 4) && (i == 2 || i == 6)) || (jj==2 && (i==0 || i==4 || i==8)))) 
				{
					if(verbose) cerr<<"invalid BX position at ("<<tj<<","<<ti<<")"<<endl;
					return false;
				}
			}
			if(chess == "RX")
			{
				int jj = 9-j;
				if(!(((jj==0 || jj == 4) && (i == 2 || i == 6)) || (jj==2 && (i==0 || i==4 || i==8)))) 
				{
					if(verbose) cerr<<"invalid RX position at ("<<tj<<","<<ti<<")"<<endl;
					return false;
				}
			}
			if(chess == "BS")
			{
				int jj = j;
				int dist = ABS(i-4) + ABS(jj-1);
				if(dist != 0 && dist != 2)
				{
					if(verbose) cerr<<"invalid BS position at ("<<tj<<","<<ti<<")"<<endl;
					return false;
				}
			}
			if(chess == "RS")
			{
				int jj = 9-j;
				int dist = ABS(i-4) + ABS(jj-1);
				if(dist != 0 && dist != 2) 
				{
					if(verbose) cerr<<"invalid RS position at ("<<tj<<","<<ti<<")"<<endl;
					return false;
				}
			}
			if(chess == "BZ")
			{
				int jj = j;
				if(jj < 3) return false;
				if((jj == 3 || jj==4) && i%2 == 1)
				{
					if(verbose) cerr<<"invalid BZ position at ("<<tj<<","<<ti<<")"<<endl;
					return false; 
				}
			}
			if(chess == "RZ")
			{
				int jj = 9 - j;
				if(jj < 3) return false;
				if((jj == 3 || jj==4) && i%2 == 1)
				{
					if(verbose) cerr<<"invalid RZ position at ("<<tj<<","<<ti<<")"<<endl;
					return false; 
				}
			}
		}
	}
	for(int id = 0; id < 14; id++) 
	{
		string chess = chessnames[id];
		if((chess == "BC" || chess == "RC" || chess == "BM" || chess == "RM" || chess == "BP" || chess == "RP" ||
		    chess == "BX" || chess == "RX" || chess == "BS" || chess == "RS") && chessNum[chess] > 2)
		{
			if(verbose) cerr<<"invalid chess number"<<endl;
			return false;
		}
		if((chess == "BJ" || chess == "RJ") && chessNum[chess] > 1) 
		{
			if(verbose) cerr<<"invalid Jiang number"<<endl;
			return false;
		}
		if((chess == "BZ" || chess == "RZ") && chessNum[chess] > 5) 
		{
			if(verbose) cerr<<"invalid Zu number"<<endl;
			return false;
		}
	}
	return true;
}

bool isfenvalid(string prev_fenstr, string cur_fenstr)
{
	assert(prev_fenstr != "");
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
	if(diffposes.size() == 0) return true;
	if(diffposes.size() == 2)
	{
		int pos1 = diffposes[0];
		int pos2 = diffposes[1];
		int i1 = pos1 % 9, j1 = pos1 / 9;
		int i2 = pos2 % 9, j2 = pos2 / 9;
		if(cur_matrix[j1][i1] != "  " && cur_matrix[j2][i2] != "  ") 
		{
			if(verbose) cerr<<"Invalid eat1"<<endl;
			return false;
		}
		if(cur_matrix[j1][i1] == "  " && cur_matrix[j2][i2] == "  ") 
		{
			if(verbose) cerr<<"Invalid eat2"<<endl;
			return false;
		}
		if(cur_matrix[j1][i1] != "  " && prev_matrix[j2][i2] != cur_matrix[j1][i1])
		{
			if(verbose) cerr<<"Eat chess changed"<<endl;
			return false;
		}
		if(cur_matrix[j2][i2] != "  " && prev_matrix[j1][i1] != cur_matrix[j2][i2]) 
		{
			if(verbose) cerr<<"Eat chess changed"<<endl;
			return false;
		}

		string movechess;
		if(cur_matrix[j1][i1] != "  ") movechess = cur_matrix[j1][i1];
		else movechess = cur_matrix[j2][i2];
		if(movechess == "BZ" || movechess == "RZ")
		{
			if(ABS(i1-i2) + ABS(j1-j2) != 1)
			{
				if(verbose) cerr<<"Error Zu move"<<endl;
				return false;
			}
		}
		if(movechess == "BC" || movechess == "RC" || movechess == "BP" || movechess == "RP")
		{
			if(i1 != i2 && j1 != j2) 
			{
				if(verbose) cerr<<"Error Che or Pao move"<<endl;
				return false;
			}
		}
		if(movechess == "BM" || movechess == "RM")
		{
			if(ABS(i1-i2) + ABS(j1-j2) != 3 || ABS(i1-i2) == 0 || ABS(j1-j2) == 0 ) 
			{
				if(verbose) cerr<<"Error Ma move"<<endl;
				return false;
			}
		}
		if(movechess == "BX" || movechess == "RX")
		{
			if(ABS(i1-i2) != 2 || ABS(j1-j2) != 2)
			{
				if(verbose) cerr<<"Error Xiang move"<<endl;
				return false;
			}
		}
		if(movechess == "BS" || movechess == "RS")
		{
			if(ABS(i1-i2) != 1 && ABS(j1-j2) != 1) 
			{
				if(verbose) cerr<<"Error Shi move"<<endl;
				return false;
			}
		}
		return true;
	}
	if(verbose) cerr<<"larger than two changes"<<endl;
	return false;
}

int main(int argc, char ** argv)
{
	if(argc == 1)
	{
		cerr<<"Usage: "<<argv[0]<<" <prev_fenstr> <cur_fenstr>"<<endl;
		return 0;
	}
	if(string(argv[argc-1]) == "-v") 
	{
		verbose = true;
		argc--;
	}
	if(argc == 2)
	{
		if(isfenvalid(argv[1]))
			cout<<"true"<<endl;
		else
			cout<<"false"<<endl;
		return 0;
	}
	if(argc == 3) 
	{
		if(isfenvalid(argv[1], argv[2]))
			cout<<"true"<<endl;
		else
			cout<<"false"<<endl;
	}
	return 0;
}
