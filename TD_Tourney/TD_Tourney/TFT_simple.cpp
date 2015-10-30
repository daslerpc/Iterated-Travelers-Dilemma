#include "TFT_simple.h"


TFT_simple::TFT_simple(int offset_val)
{
	first = true;
	offset = offset_val;
	logging = true;

	name = "TFT - Simple (y";
	short_name = "TFT (y";

	if(offset>0)
	{
		name.append("+");
		short_name.append("+");
	}

	char temp[32];	// buffer to hold offset string below
	
	_itoa_s (offset, temp, 10);	// change the offset value to a string
	name.append(temp);
	short_name.append(temp);

	name.append(")");
	short_name.append(")");
}

TFT_simple::~TFT_simple(void)
{
}

int TFT_simple::getBid_Extended()
{
	int bid = 0;

	if(first)
	{
		first = false;
		bid = randomBid();
	}
	else
	{
		bid = opp_bid + offset;
	}

	return bid;
}

void TFT_simple::end_Match_Extended()
{
	opp_bid = 0;
	first = true;
}
