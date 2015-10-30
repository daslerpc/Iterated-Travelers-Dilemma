#pragma once
#include "traveler.h"
class always :
	public traveler
{
public:
	always(int x);

	always(void);
	~always(void);

	int getBid_Extended();

	int always_num;
};

