#pragma once
#include "traveler.h"
class TFT_predict :
	public traveler
{
public:
	TFT_predict(int strategy);
	TFT_predict(void);
	TFT_predict(bool no_log);
	~TFT_predict(void);

	int getBid_Extended();
	void end_Match_Extended();

private:
	bool first;

	bool a;
	bool b;
	bool c;
	bool d;
	bool e;
};

