#pragma once

#include <string>
#include <fstream>
#include <vector>
using namespace std;

typedef enum strat_groups {
	RAND,	
	ALWAYS,
	BUCKET_PROB,
	BUCKET_DET,
	QLEARN_SHORT,
	QLEARN_LONG,
	SIMPLE_TREND,
	MIXED,
	TFT_SIMPLE,
	TFT_COMPLEX,
	TFT_MIXED,
	ZEUTHEN,
	NUM_GROUPS
} strat_groups;

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

