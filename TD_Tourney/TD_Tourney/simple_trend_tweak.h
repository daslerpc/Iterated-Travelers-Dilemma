#pragma once
#include "traveler.h"

#include <vector>

class simple_trend_tweak :
	public traveler
{
public:
	simple_trend_tweak(int window_size, double slope_threshold);

	simple_trend_tweak(void);
	~simple_trend_tweak(void);

	int simple_trend_tweak::getBid_Extended();
	void simple_trend_tweak::return_Round_Results_Extended(int settlement, int opponents_bid);
	void simple_trend_tweak::end_Match_Extended();

private:
	int k;
	double eps;

	int dumb_rounds;

	int index_sum;
	int index_squared_sum;
	int value_sum;
	int index_value_product_sum;

	vector<int> history;
};

