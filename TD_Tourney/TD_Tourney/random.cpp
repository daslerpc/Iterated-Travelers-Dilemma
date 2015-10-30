#include "random.h"

#include <math.h>  // for the ceil function

random::random(int first_val, int last_val)
{
	name = "Random [";
	short_name = "R[";
	logging = true;


	first = first_val;
	last = last_val;

	char temp[32];	// buffer to hold k string below
	_itoa_s (first_val, temp, 10);	// change the first value to a string
	name.append(temp);
	short_name.append(temp);

	name.append(", ");
	short_name.append(", ");

	_itoa_s (last_val, temp, 10);	// change the last value to a string
	name.append(temp);
	short_name.append(temp);

	name.append("]");
	short_name.append("]");
}


random::~random(void)
{
}

int random::getBid_Extended()
{
	int range = last - first + 1;
	int bid_count = ceil(double(range)/double(GRANULARITY));

	my_bid = rand() % bid_count;
	my_bid = my_bid * GRANULARITY + first;

	return my_bid;
}