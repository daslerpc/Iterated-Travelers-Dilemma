#pragma once
#include "traveler.h"
class random :
	public traveler
{
public:
	random(int first=MIN_BID, int last=MAX_BID);
	~random(void);

	int getBid_Extended();

	int first;
	int last;
};

