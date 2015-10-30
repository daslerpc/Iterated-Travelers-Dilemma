#pragma once

#include <vector>
#include <fstream>  // For file logging

#include "traveler.h"

extern ofstream error;

class tournament
{
public:
	tournament(void);
	~tournament(void);

	void runTournament(int matches_to_play=100, int rounds_to_play=1000);
	void addCompetitors();
	void sortCompetitors(int utility);
	void printRanking(int utility);
	void logRanking(int utility);
	void TeXlogRanking(int utility);
	
public:
	ofstream log;
	ofstream TeXlog;

	int rounds_to_play;
	int matches_to_play;

	int median_bid;
	
	// variables for tracking tournament progress
	double current_match;
	double total_matches;
	int last_percent;
	int new_percent;

	vector<traveler *> competitors;	
	vector<traveler *> twins;	

	pair<int, int> getPayoffs(int xBid, int yBid);
	void normalizeUtils();
	void playMatches();
	void playTwins();

	void printPercentComplete();
};

