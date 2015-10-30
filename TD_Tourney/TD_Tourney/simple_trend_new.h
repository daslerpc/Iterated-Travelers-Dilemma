#pragma once
#include "traveler.h"

#include <vector>

class simple_trend_new :
	public traveler
{
public:
	simple_trend_new(int window_size, double slope_threshold);

	simple_trend_new(void);
	~simple_trend_new(void);

	int simple_trend_new::getBid_Extended();
	void simple_trend_new::return_Round_Results_Extended(int settlement, int opponents_bid);
	void simple_trend_new::end_Match_Extended();

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

