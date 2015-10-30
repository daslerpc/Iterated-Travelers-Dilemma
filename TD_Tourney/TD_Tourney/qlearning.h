#pragma once
#include "traveler.h"

#include <vector>

enum comparison {LOW, EQUAL, HIGH, COMP_TYPE_SIZE};

class qlearning :
	public traveler
{
public:
	qlearning(double learning_rate, double discount);
	qlearning(void);
	~qlearning(void);

	void return_Round_Results_Extended(int settlement, int opponents_bid);
	
	int getBid_Extended();
	void end_Match_Extended();

private:	
	double bestActionReward();


	const int history_size;
	const double q_init;

	double learning_rate;
	double discount;

	int not_primed;

	vector<vector<vector<vector<vector<vector<double>>>>>> Q;
	vector<comparison> history;	 // holds a state consisting of 5 opponent bids
};

