#pragma once

#include <string>
#include <fstream> // For file logging
#include <windows.h>

using namespace std;

extern const int MIN_BID;
extern const int MAX_BID;
extern const int GRANULARITY;
extern const int BONUS_MALUS;

extern const int MATCHES;
extern const int ROUNDS;

extern ofstream error;
extern string params;

class traveler
{
public:
	int pay_out;
	int match_sum;

	double Utils[4];

	int my_bid;
	int opp_bid;
	
	ofstream log;
	bool logging;

	string name;
	string short_name;

	traveler(void);
	~traveler(void);

	void return_Round_Results(int settlement, int opponents_bid);
	void return_Match_Results(int opponents_match_sum);
	void print();
		
	int getBid();
	void start_Match(string opponent_name, int match_num);
	void end_Match();

	void start_Opponent(string opponent_name);
	void end_Opponent();

	int randomBid();
	int matchSum();

private:
	virtual void return_Round_Results_Extended(int settlement, int opponents_bid);
	virtual void return_Match_Results_Extended(int opponents_match_sum);

	virtual int getBid_Extended();
	virtual void start_Match_Extended(string opponent_name);
	virtual void end_Match_Extended();
};

