#include "simple_trend_new.h"


simple_trend_new::simple_trend_new(int k_in, double eps_in)
{
	k = k_in;
	eps = eps_in;
	logging = true;

	index_sum = 0;
	index_squared_sum = 0;	
	value_sum = 0;
	index_value_product_sum = 0;

	dumb_rounds = k;

	for(int x=0; x<k; x++)
	{
		index_sum += x;
		index_squared_sum += x*x;	
		history.push_back(0);
	}

	char temp[32];	// buffer to hold k string below

	name = "Simple Trend New - K = ";
	
	_itoa_s (k_in, temp, 10);	// change the k value to a string
	name.append(temp);

	name.append(", Eps = ");

	int dummy = (int)eps;
	double temp_eps = eps;
	
	_itoa_s (dummy, temp, 10);	// change the epsilon value to a string
	name.append(temp);

	temp_eps-=dummy;

	name.append(".");
	temp_eps*=10;
	dummy = (int)temp_eps;
	
	do
	{
		_itoa_s (dummy, temp, 10);	// change the epsilon value to a string
		name.append(temp);

		temp_eps-=dummy;

		temp_eps*=10;
		dummy = (int)temp_eps;
	}
	while(temp_eps!=0);
}

simple_trend_new::simple_trend_new(void)
{
	name = "Simple Unitialized";
}

simple_trend_new::~simple_trend_new(void)
{
}

int simple_trend_new::getBid_Extended()
{
	int bid = 0;
	
	if(dumb_rounds > 0)
	{
		dumb_rounds--;
		bid = randomBid();
	}
	else
	{
		double m = double(k*index_value_product_sum - index_sum*value_sum)/double(k*index_squared_sum - index_sum*index_sum);
		
		if( m > eps )
		{
			if(opp_bid < my_bid)
				bid = my_bid;
			else
				bid = my_bid + GRANULARITY;
		}
		else if ( m < -eps)
			bid = MIN_BID;
		else
		{
			if( rand() % 100 < 80)  //i.e. 80% of the time
				bid = opp_bid - GRANULARITY;
			else
				bid = MAX_BID;
		}

		log << "m = " << m << "\t\t";
	}

	return bid;
}

void simple_trend_new::return_Round_Results_Extended(int settlement, int opponents_bid)
{
	value_sum -= history[0];  //history[0] is leaving the sliding window, remove it from the sum
	index_value_product_sum = 0;  //calculate new index_value_product_sum
	
	for(int x=0; x<k-1; x++)
	{
		history[x] = history[x+1];		//move every element in the window buffer back one
		index_value_product_sum += x*history[x];  //then take the index_value product and add it to the sum
	}
	
	history[k-1] = settlement;  //our newest payout is put in the end of our window buffer
	
	value_sum += history[k-1];  //our newest payout is added to the value sum
	index_value_product_sum += (k-1)*history[k-1];  //add the newest index_value product
}

void simple_trend_new::end_Match_Extended()
{
	value_sum = 0;
	index_value_product_sum = 0;

	for(int x=0; x<k; x++)
		history[x] = 0;

	dumb_rounds = k;
}