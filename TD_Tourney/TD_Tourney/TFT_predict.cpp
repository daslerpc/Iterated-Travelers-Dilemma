#include "TFT_predict.h"

#include <iostream>


TFT_predict::TFT_predict(int strategy)
{
	logging = true;
	first = true;
	
	my_bid = 0;

	a = b = c = d = e = false;

	name = "TFT - ";
	short_name = "";

	switch(strategy)
	{
	case 1: 
		a = true;
		name.append("Low(x) Equal(x) High(y-g)");
		short_name.append("L(x) E(x) H(y-g)");
		break;
	case 2: 
		b = true;
		name.append("Low(x) Equal(x-2g) High(y-g)");
		short_name.append("L(x) E(x-2g) H(y-g)");
		break;
	case 3: 
		c = true;
		name.append("Low(x-2g) Equal(x) High(y-g)");
		short_name.append("L(x-2g) E(x) H(y-g)");
		break;
	case 4: 
		d = true;
		name.append("Low(x-2g) Equal(x-2g) High(y-g)");
		short_name.append("L(x-2g) E(x-2g) H(y-g)");
		break;
	case 5: 
		e = true;
		name.append("Low(y-g) Equal(x-g) High(x-g)");
		short_name.append("L(y-g) E(x-g) H(x-g)");
		break;
	}
}

TFT_predict::TFT_predict(bool no_log)
{
	first = true;
	name = "TFT - Predict";

	my_bid = 0;
}


TFT_predict::~TFT_predict(void)
{
}

int TFT_predict::getBid_Extended()
{
	int bid = 0;

	if(first)
	{
		first = false;
		bid = randomBid();
	}
	else
	{
		if(my_bid > opp_bid)
			bid = opp_bid*(a||b||c||d) + my_bid*e - GRANULARITY;
		else
		{
			if(e)
			{
				if(my_bid < opp_bid)
					bid = max(opp_bid - GRANULARITY, MIN_BID);	
				if(my_bid == opp_bid)
					bid = max(opp_bid - GRANULARITY, MIN_BID);	
			}
			else
			{
				if(my_bid < opp_bid)
					bid = max(my_bid - 2*GRANULARITY*(c||d), MIN_BID);	
				if(my_bid == opp_bid)
					bid = max(my_bid - 2*GRANULARITY*(b||d), MIN_BID);	
			}
		}
	}

	my_bid = bid;
	return bid;
}

void TFT_predict::end_Match_Extended()
{
	my_bid = 0;
	opp_bid = 100;
	first = true;
}