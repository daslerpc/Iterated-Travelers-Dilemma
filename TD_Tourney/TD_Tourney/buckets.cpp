#include "buckets.h"

#include <iostream>

//If this constructor gets called, then we forgot to initialize this instance
buckets::buckets(int method_in, double retention_rate, boolean one_under)
{
	method = method_in;
	logging = true;
	play_one_under = one_under;

	if(play_one_under)
		name = "Under ";
	else
		name = "";
	
	switch(method)
	{
	case 0:
		name += "Buckets - (Fullest, Highest)";
		break;
	case 1:
		name += "Buckets - (Fullest, Lowest)";
		break;
	case 2:
		name += "Buckets - (Fullest, Random)";
		break;
	case 3:
		name += "Buckets - (Fullest, Newest)";
		break;
	default:
		if(retention_rate == 0)
			name += "Buckets - PD, no age bias";
		else
		{
			name += "Buckets - PD, Retention = ";

			char temp[32];	// buffer to hold string below
			int dummy = (int)retention_rate;
			double temp_rate = retention_rate;
			
			_itoa_s (dummy, temp, 10);	// change the retention_rate value to a string
			name.append(temp);

			temp_rate-=dummy;

			name.append(".");
			temp_rate*=10;
			dummy = (int)temp_rate;
			
			do
			{
				_itoa_s (dummy, temp, 10);	// change the retention_rate value to a string
				name.append(temp);

				temp_rate-=dummy;

				temp_rate*=10;
				dummy = (int)temp_rate;
			}
			while(temp_rate!=0);
		}
	}
	

	for(int x=0; x<=MAX_BID; x++)
		opp_bids.push_back(0);

	first = true;

	bucket_total = 0;

	fullest_value = 0;

	retain_rate = retention_rate;
}

buckets::buckets(void)
{
	name = "Buckets Unitialized";
}


buckets::~buckets(void)
{
}


int buckets::getBid_Extended()
{
	int bid;

	if(first)
	{
		first = false;
		bid = randomBid();
	}
	else
	{
		if(method == 4) //If we're using the probability distro.
		{
			double value = (double)rand()/(double)RAND_MAX * bucket_total;
			double sum = 0.0;
			unsigned int lcv = 0;

			while( (lcv < opp_bids.size()) && (sum < value) )
			{
				sum += opp_bids[lcv];
				lcv++;
			}

			bid = lcv - 1;  //minus one for the extra lcv++ at the end of the loop
		}
		else
			bid = fullest_IDs[0];

		if(play_one_under)
			bid -= GRANULARITY;
	}

	return bid;
}

void buckets::return_Round_Results_Extended(int settlement, int opponents_bid)
{
	if(method == 4) //If we're using the probability distro.
	{
		for(unsigned int lcv = 0; lcv<opp_bids.size(); lcv++)
			opp_bids[lcv]*=retain_rate;

		opp_bids[opponents_bid]++;

		bucket_total = bucket_total*retain_rate + 1;
	}
	else
	{
		opp_bids[opponents_bid]++;

		if(opp_bids[opponents_bid] > fullest_value)
		{
			fullest_IDs.clear();
			fullest_IDs.push_back(opponents_bid);

			fullest_value = (int)opp_bids[opponents_bid];
		}
		else if (opp_bids[opponents_bid] == fullest_value)
		{
			fullest_IDs.push_back(opponents_bid);
			int temp;
			int rand_ID;

			//sort for tie-breaker
			switch(method)
			{
			case 0:  //Highest value bucket wins
				if(fullest_IDs[fullest_IDs.size()-1] > fullest_IDs[0])
				{
					temp = fullest_IDs[fullest_IDs.size()-1];
					fullest_IDs[fullest_IDs.size()-1] = fullest_IDs[0];
					fullest_IDs[0] = temp;
				}
				break;
			case 1:  //Lowest value bucket wins
				if(fullest_IDs[fullest_IDs.size()-1] < fullest_IDs[0])
				{
					temp = fullest_IDs[fullest_IDs.size()-1];
					fullest_IDs[fullest_IDs.size()-1] = fullest_IDs[0];
					fullest_IDs[0] = temp;
				}
				break;
			case 2:  //Random wins
				rand_ID = rand() % fullest_IDs.size();
				temp = fullest_IDs[rand_ID];
				fullest_IDs[rand_ID] = fullest_IDs[0];
				fullest_IDs[0] = temp;
				break;
			case 3:  //Newest wins
				temp = fullest_IDs[fullest_IDs.size()-1];
				fullest_IDs[fullest_IDs.size()-1] = fullest_IDs[0];
				fullest_IDs[0] = temp;
				break;
			}
		}
	}
}

void buckets::end_Match_Extended()
{
	first = true;
	bucket_total = 0;
	fullest_value = 0;

	for(unsigned int lcv = 0; lcv < opp_bids.size(); lcv++)
		opp_bids[lcv] = 0;
}
