#pragma once
#include "traveler.h"
#include <vector>

class mixed :
	public traveler
{
public:
//	mixed(int TFT_predict, int mix1, int always1, int mix2, int always2=0, int mix3=0);
	mixed(traveler* first_strategy, int mix1, traveler* second_strategy, int mix2, traveler* third_strategy=0, int mix3=0);

	mixed(void);
	~mixed(void);

	void return_Round_Results_Extended(int settlement, int opponents_bid);
	void end_Match_Extended();

	int getBid_Extended();

	int always_num1;
	int always_num2;

	int chance1;
	int chance2;
	int chance3;

	vector<int> bucketsr;  //Used for random value selection

	traveler* strategies[3];
};

