#pragma once
#include "traveler.h"
class TFT_simple :
	public traveler
{
public:
	TFT_simple(int offset_val = -GRANULARITY);
	~TFT_simple(void);

	int getBid_Extended();
	void end_Match_Extended();

private:
	bool first;
	int offset;
};


