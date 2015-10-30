#pragma once

#include <string>
#include <fstream>
#include <vector>
using namespace std;

class strategy
{
public:
	strategy(void);
	~strategy(void);
	strategy(int epoch_size);

	void getScores(string Opponent_Name);
	void setName(string name);

	void printScores();
	void logScores();

	int epoch_size;
	int epoch_num;
	string name;
	vector<double> scores;
};

