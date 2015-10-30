#pragma once
#include "traveler.h"

#include <vector>

class buckets :
	public traveler
{
public:
	buckets(int method, double retain_rate = 1.0, boolean one_under = true);

	buckets(void);
	~buckets(void);

	int getBid_Extended();
	void return_Round_Results_Extended(int settlement, int opponents_bid);

	void end_Match_Extended();

private:
	bool first;
	bool play_one_under;

	int method;

	double bucket_total;

	double retain_rate;

	int fullest_value;
	vector<int> fullest_IDs;

	vector<double> opp_bids;
};

