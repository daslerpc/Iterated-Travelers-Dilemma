#include "Zeuthen.h"
#include <iostream>

Zeuthen::Zeuthen(bool be_positive)
{
	logging = true;
	first = true;
	positive = be_positive;
	
	opp_last_bid = 0;
	my_last_bid = 0;

	my_risk = 0;
	opp_risk = 0;

	if(positive)
		name = "Zeuthen Strategy - Positive";
	else
		name = "Zeuthen Strategy - Negative";
}

Zeuthen::~Zeuthen(void)
{
}

void Zeuthen::return_Round_Results_Extended(int settlement, int opponents_bid)
{
	opp_last_bid = opponents_bid;
}

int Zeuthen::getBid_Extended()
{
	int bid = my_last_bid; 

	if(first)
	{
		first = false;
		bid = MAX_BID - GRANULARITY;
	}
	else
	{
		my_risk = calculate_Risk(bid, opp_last_bid);
		opp_risk = calculate_Risk(opp_last_bid, bid);
			
		if( my_risk <= opp_risk )
			bid = find_minimal_sufficient_concession(bid, opp_last_bid);		
	}

	my_last_bid = bid;
	return bid;
}


void Zeuthen::end_Match_Extended()
{
	first = true;
	
	opp_last_bid = 0;
	my_last_bid = 0;

	my_risk = 0;
	opp_risk = 0;
}

double Zeuthen::calculate_Risk(int bid1, int bid2)
{
	double risk = 0;

	int utility_my_proposal;
	int utility_opp_proposal;
	int utility_conflict_loss;

	// calculate the utility for me if my proposal is accepted
	if(bid1 == MAX_BID)
		utility_my_proposal = MAX_BID;
	else
		utility_my_proposal = bid1 + BONUS_MALUS;

	// calculate the utility for me if the opponents proposal is accepted
	if(bid2 == MAX_BID)
		utility_opp_proposal = MAX_BID;
	else
		utility_opp_proposal = bid2 - BONUS_MALUS;

	// calculate the utility lost accepting the conflict deal (MIN_BID, MIN_BID) instead of conceding
	utility_conflict_loss = utility_opp_proposal - MIN_BID;

	// calculate my risk
	if(utility_conflict_loss == 0)
		risk = 1;
	else
		risk = (double)(utility_my_proposal - utility_opp_proposal) / (double)utility_conflict_loss;

	return risk;
}


int Zeuthen::find_minimal_sufficient_concession(int my_bid, int opp_last_bid)  
{
	int concession = MIN_BID;
	
	if(positive)
		concession = my_bid;

	double my_risk = calculate_Risk(concession, opp_last_bid);
	double opp_risk = calculate_Risk(opp_last_bid, concession);

	while(my_risk <= opp_risk && concession < MAX_BID)
	{
		concession += GRANULARITY;

		my_risk = calculate_Risk(concession, opp_last_bid);
		opp_risk = calculate_Risk(opp_last_bid, concession);
	}

	return concession;
}