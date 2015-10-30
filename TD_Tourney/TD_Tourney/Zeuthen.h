#pragma once
#include "traveler.h"
class Zeuthen :
	public traveler
{
public:
	Zeuthen(bool positive=true);
	~Zeuthen(void);

private:
	bool first;
	bool positive;

	int opp_last_bid;
	int my_last_bid;
	
	double my_risk;
	double opp_risk;

	void return_Round_Results_Extended(int settlement, int opponents_bid);
	int getBid_Extended();

	void end_Match_Extended();

	double calculate_Risk(int bid1, int bid2);
	int find_minimal_sufficient_concession(int my_bid, int opp_last_bid);
};

