#include "mixed.h"
#include <vector>
#include <assert.h>

#include <iostream>

#include "TFT_simple.h"
#include "TFT_predict.h"

mixed::mixed(traveler* first_strategy, int mix1, traveler* second_strategy, int mix2, traveler* third_strategy, int mix3)
{
	logging = true;

	chance1 = mix1;
	chance2 = mix2;
	chance3 = mix3;

	assert(100 == chance1 + chance2 + chance3);


	strategies[0] = first_strategy;
	strategies[1] = second_strategy;
	strategies[2] = third_strategy;

	strategies[0]->logging = false;
	strategies[1]->logging = false;
	
	if(strategies[2])
		strategies[2]->logging = false;

	int x = 0;

	for(x; x < chance1; x++)
		bucketsr.push_back(0);
	for(x; x < chance1 + chance2; x++)
		bucketsr.push_back(1);
	for(x; x < chance1 + chance2 + chance3; x++)
		bucketsr.push_back(2);
	
	char temp[4];	// buffer to hold "mixed" string below

	name = "Mixed - {";
	
	name.append(strategies[0]->short_name);
	name.append(", ");
	
	_itoa_s (mix1, temp, 10);	// change the "mixed" value to a string
	name.append(temp);
	name.append("%); (");

	name.append(strategies[1]->short_name);
	name.append(", ");
	
	_itoa_s (mix2, temp, 10);	// change the "mixed" value to a string
	name.append(temp);
	
	if(strategies[2])
	{
		name.append("%); (");

		name.append(strategies[2]->short_name);
		name.append(", ");

		_itoa_s (mix3, temp, 10);	// change the "mixed" value to a string
		name.append(temp);
	}

	name.append("%)}");
}

//If this constructor gets called, then we forgot to initialize this instance
mixed::mixed(void)
{
	name = "Mixed Unitialized";
}


mixed::~mixed(void)
{
	for(int x=0; x<3; x++)
	{
		if(strategies[x])
			delete strategies[x];
	}
}

int mixed::getBid_Extended()
{
	int pick = bucketsr[rand() % 100];
	
	my_bid = strategies[pick]->getBid();
	
	strategies[0]->my_bid = my_bid;
	strategies[1]->my_bid = my_bid;

	if(strategies[2])
		strategies[2]->my_bid = my_bid;

	return my_bid;
}

void mixed::return_Round_Results_Extended(int settlement, int opponents_bid)
{
	strategies[0]->return_Round_Results(settlement, opponents_bid);
	strategies[1]->return_Round_Results(settlement, opponents_bid);

	if(strategies[2])
		strategies[2]->return_Round_Results(settlement, opponents_bid);
}

void mixed::end_Match_Extended()
{
	strategies[0]->end_Match();
	strategies[1]->end_Match();
	
	if(strategies[2])
		strategies[2]->end_Match();
}
