#include "always.h"

#include <assert.h>

always::always(int x)
{
	logging = true;
	always_num = max(min(x, MAX_BID), MIN_BID);
	char temp[4];	// buffer to hold "always" string below

	assert(always_num % GRANULARITY == 0);

	name = "Always ";
	short_name = "";
	
	_itoa_s (x, temp, 10);	// change the "always" value to a string
	name.append(temp);
	short_name.append(temp);
}

//If this constructor gets called, then we forgot to initialize this instance
always::always(void)
{
	always_num = -1;
	name = "Always Unitialized";
}


always::~always(void)
{
}


int always::getBid_Extended()
{
	return always_num;
}

