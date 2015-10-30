#include "traveler.h"

#include <iostream>
#include <string>
#include <fstream> // For file logging
#include <math.h> //  For floor

traveler::traveler(void)
{
	pay_out = 0;
	match_sum = 0;
	Utils[0] = 0;
	Utils[1] = 0;
	Utils[2] = 0;
	Utils[3] = 0;

	opp_bid = 0;

	name = "Generic Traveler";
	short_name = "Gnrc Trvlr";
}


traveler::~traveler(void)
{
}

void traveler::return_Round_Results(int settlement, int opponents_bid)
{
	pay_out = settlement;
	opp_bid = opponents_bid;

	match_sum += settlement;
	Utils[0] += settlement;

	int ideal_payout;
	int ideal_bid = max(MIN_BID, opponents_bid - GRANULARITY);
	
	if(ideal_bid == opponents_bid)
		ideal_payout = ideal_bid;
	else
		ideal_payout = ideal_bid + BONUS_MALUS;

	Utils[2] += (double)settlement/(double)ideal_payout;
	Utils[3] += 1.0 - double(abs(my_bid - ideal_bid))/double(MAX_BID - MIN_BID);

	if(logging)
		log << "{" << my_bid << ", " << opp_bid << "} -> (" << settlement << ")\t";
	
	return_Round_Results_Extended(settlement, opponents_bid);

	if(logging)
		log << endl;
}

void traveler::return_Match_Results(int opponents_match_sum)
{
	if(match_sum > opponents_match_sum)
		Utils[1]++;
	else if(match_sum < opponents_match_sum)
		Utils[1]--;
}

void traveler::return_Round_Results_Extended(int settlement, int opponents_bid)
{
}

void traveler::return_Match_Results_Extended(int opponents_match_sum)
{
}


int traveler::getBid()
{
	my_bid = getBid_Extended();
	my_bid = min(MAX_BID, max(MIN_BID, my_bid));
	return my_bid;
}

void traveler::print()
{
	cout << Utils[0] << ":\t" << name << endl;
}

int traveler::getBid_Extended()
{
	return -1; //This value gets wiped out by the range check in getBid
}

void traveler::end_Match()
{
	match_sum = 0;
	end_Match_Extended();
	if(logging)
		log << endl << endl;
}

void traveler::end_Match_Extended()
{
}

void traveler::start_Match(string opponent_name, int match_num)
{
	if(logging)
		log << "Match " << match_num << endl;	

	start_Match_Extended(opponent_name);
}

void traveler::start_Opponent(string opponent_name)
{
	if(logging)
	{
		string filename = "logs/";
		filename.append(name);
	
		CreateDirectory(filename.c_str(), NULL);

		filename.append("/");
		filename.append(name);
		filename.append(" vs ");
		if(opponent_name == name)
			filename.append("Twin");
		else
			filename.append(opponent_name);
		filename.append(".txt");
		log.open(filename.c_str());

		if(!log.is_open())
			error << "Failed to open file \""<< filename << "\"" << endl;

		log << name << " vs. " << opponent_name << endl << endl;
	}
}

void traveler::end_Opponent()
{
	if(log.is_open())
		log.close();
}

void traveler::start_Match_Extended(string opponent_name)
{
}

int traveler::randomBid()
{
	int bid = 0;
	int range = MAX_BID - MIN_BID + 1;
	int bid_count = int(ceil(double(range)/double(GRANULARITY)));
	bid = rand() % bid_count;

	bid = bid * GRANULARITY + MIN_BID;

	return bid;
}

int traveler::matchSum()
{
	return match_sum;
}
