#include "tournament.h"

#include <time.h>	 // For time (used to initialize rand
#include <iostream>  // For cout
#include <math.h>	// For math functions (e.g. pow)
#include <stdlib.h> // For clearing the screen
#include <assert.h>
#include <string>

#include "traveler.h"

#include "always.h"
#include "buckets.h"
#include "mixed.h"
#include "qlearning.h"
#include "random.h"
#include "simple_trend.h"
#include "simple_trend_tweak.h"
#include "simple_trend_new.h"
#include "TFT_simple.h"
#include "TFT_predict.h"
#include "Zeuthen.h"

using namespace std;

enum util {U1, U2, U3, U3p};

/*For the different TFT strategies used in the Mixed strategies
TFT_SIMPLE:  Play y-1 always.
A:  x<y -> x;		x=y -> x;		x>y -> y-1;
B:	x<y -> x;		x=y -> x-2;		x>y -> y-1;
C:	x<y -> x-2;		x=y -> x;		x>y -> y-1;
D:	x<y -> x-2;		x=y -> x-2;		x>y -> y-1;
*/
enum tft_type { TFT_SIMPLE, A, B, C, D, E };

/*Bucket method choices
FW_HIGHEST:	Fullest bucket wins, highest bucket wins ties
FW_LOWEST:	Fullest bucket wins, lowest bucket wins ties
FW_RANDOM:	Fullest bucket wins, random bucket wins ties
FW_NEWEST:	Fullest bucket wins, newest tying bucket wins ties
PD:			Probability distribution
*/
enum bucket_type { FW_HIGHEST, FW_LOWEST, FW_RANDOM, FW_NEWEST, PD };

//Zeuthen method (positive/negative concession)
enum zeuthen_type { NEG, POS };


tournament::tournament()
{
	assert(MIN_BID % GRANULARITY == 0);
	assert(MAX_BID % GRANULARITY == 0);

	median_bid = (MIN_BID + MAX_BID)/2;
	median_bid += median_bid % GRANULARITY;
	
	current_match = 0;
	last_percent = 0;
	new_percent = 0;
	
	string filename = "logs/";

	filename.append("Tournament Results.txt");
	log.open(filename.c_str());

	filename = "logs/";

	filename.append("results ");
	filename.append(params);
	filename.append(".tex");

	TeXlog.open(filename.c_str());

	TeXlog.precision(6);
	TeXlog.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed

	/* initialize random seed: */
	 srand ( (unsigned int)time(NULL) );
}

tournament::~tournament(void)
{
	if(log.is_open())
		log.close();
	
	if(TeXlog.is_open())
		TeXlog.close();

	if(error.is_open())
		error.close();
}

void tournament::runTournament(int matches, int rounds)
{
	rounds_to_play = rounds;
	matches_to_play = matches;

	log << "Traveler's Dilemma Tournament with " << competitors.size() << " competitors." << endl << endl;
	log << "Each competitor plays each other competitor and its twin " << matches_to_play << " times." << endl;
	log << "Each match played for " << rounds_to_play << " rounds." << endl << endl;
	log << "Bids are in the interval [" << MIN_BID << ", " << MAX_BID << "]." << endl; 
	log << "Granularity: " << GRANULARITY << endl;
	log << "Bonus/Malus: " << BONUS_MALUS << endl;
	log << endl << endl;

	TeXlog << "\\newpage  " << endl;
	TeXlog << "\\section{Tournament Rankings for $\\delta = " << BONUS_MALUS << "$}\\label{results_" << BONUS_MALUS <<"}" << endl;
	TeXlog << "For definitions of the shorthand notation used here, see appendix \\ref{shorthand} \\\\" << endl;
	TeXlog << "\\newline" << endl;
	TeXlog << "Bids are in the interval [" << MIN_BID << ", " << MAX_BID << "].\\\\" << endl; 
	TeXlog << "Granularity: " << GRANULARITY << "\\hspace{0.25in} Bonus/Malus: " << BONUS_MALUS << "\\\\" << endl;

	playMatches();	

	normalizeUtils();

	cout << endl << endl << endl;

	for( int u=U1; u<=U3p; u++)
	{
		sortCompetitors(u);
		printRanking(u);	
		logRanking(u);
		TeXlogRanking(u);
	}
}

void tournament::playMatches()
{
	int xBid;
	int yBid;
	pair<int, int> payoffs;

	total_matches = matches_to_play*(pow(competitors.size(), 2.0) + competitors.size())/2.0;

	printPercentComplete();

	for(unsigned int x=0; x < competitors.size()-1; x++)  //For each competitor
	{
		for(unsigned int y=x+1; y < competitors.size(); y++)  //Play against all other competitors
		{
			competitors[x]->start_Opponent(competitors[y]->name);
			competitors[y]->start_Opponent(competitors[x]->name);

			for(int lcv=0; lcv<matches_to_play; lcv++)
			{
				//Let competitors know a match is starting
				competitors[x]->start_Match(competitors[y]->name, lcv+1);	
				competitors[y]->start_Match(competitors[x]->name, lcv+1);
				
				//Play all the rounds in a match
				for(int round = 0; round < rounds_to_play; round++)
				{
					xBid = competitors[x]->getBid();
					yBid = competitors[y]->getBid();

					if(xBid % GRANULARITY)
						error << "BID ERROR: Bid of " << xBid << " by " << competitors[x]->name << " not divisible by " << GRANULARITY << "." << endl;
					
					if(yBid % GRANULARITY)
						error << "BID ERROR: Bid of " << yBid << " by " << competitors[y]->name << " not divisible by " << GRANULARITY << "." << endl;

					payoffs = getPayoffs(xBid, yBid);
					
					competitors[x]->return_Round_Results(payoffs.first, yBid);
					competitors[y]->return_Round_Results(payoffs.second, xBid);
				} // end of rounds loop

				//Tell competitors that the match is over and what their opponent's total score was
				competitors[x]->return_Match_Results(competitors[y]->matchSum());
				competitors[y]->return_Match_Results(competitors[x]->matchSum());

				//Let the competitors know that the match is over
				competitors[x]->end_Match();
				competitors[y]->end_Match();

				// Calculate percent of run complete
				current_match++; //one more match completed

				printPercentComplete();
			} // end of matches loop

			competitors[x]->end_Opponent();
			competitors[y]->end_Opponent();

		} // end of opponents loop
	} // end of competitors loop

	
	playTwins();
}

void tournament::playTwins()
{
	int xBid;
	int yBid;
	pair<int, int> payoffs;

	for(unsigned int x=0; x < competitors.size() && x < twins.size(); x++)  //For each competitor
	{
		competitors[x]->start_Opponent(twins[x]->name);
		twins[x]->start_Opponent(competitors[x]->name);

		for(int lcv=0; lcv<matches_to_play; lcv++)
		{
			//Let competitors know a match is starting
			competitors[x]->start_Match(twins[x]->name, lcv+1);	
			twins[x]->start_Match(competitors[x]->name, lcv+1);

			//Play all the rounds in a match
			for(int round = 0; round < rounds_to_play; round++)
			{
				xBid = competitors[x]->getBid();
				yBid = twins[x]->getBid();
				payoffs = getPayoffs(xBid, yBid);

				competitors[x]->return_Round_Results(payoffs.first, yBid);
				twins[x]->return_Round_Results(payoffs.second, xBid);
			}

			//Tell competitors that the match is over and what their opponent's total score was
			competitors[x]->return_Match_Results(twins[x]->matchSum());
			twins[x]->return_Match_Results(competitors[x]->matchSum());

			//Let the competitors know that the match is over
			competitors[x]->end_Match();
			twins[x]->end_Match();

			// Calculate percent of run complete
			current_match++; //one more match completed

			printPercentComplete();
		}

		competitors[x]->end_Opponent();
		twins[x]->end_Opponent();
	}
}

void tournament::addCompetitors()
{
	
	competitors.push_back(new random);
	competitors.push_back(new random(MAX_BID - GRANULARITY, MAX_BID));
	competitors.push_back(new random(MAX_BID  - 2*BONUS_MALUS, MAX_BID));  // Risk minimization range
	twins.push_back(new random);
	twins.push_back(new random(MAX_BID - GRANULARITY, MAX_BID));
	twins.push_back(new random(MAX_BID  - 2*BONUS_MALUS, MAX_BID));  // Risk minimization range
	
	competitors.push_back(new always(MIN_BID));
	competitors.push_back(new always(MAX_BID - GRANULARITY));
	competitors.push_back(new always(MAX_BID));
	competitors.push_back(new always(MAX_BID - 2*BONUS_MALUS + 1));  // Risk minimization as per Halpern
	twins.push_back(new always(MIN_BID));
	twins.push_back(new always(MAX_BID - GRANULARITY));
	twins.push_back(new always(MAX_BID));
	twins.push_back(new always(MAX_BID - 2*BONUS_MALUS + 1));  // Risk minimization as per Halpern

	competitors.push_back(new TFT_simple);
	twins.push_back(new TFT_simple);

	competitors.push_back(new TFT_predict(E));
	twins.push_back(new TFT_predict(E));

	competitors.push_back(new mixed(new TFT_predict(A), 80, new always(MAX_BID), 20));
	twins.push_back(new mixed(new TFT_predict(A), 80, new always(MAX_BID), 20));

	competitors.push_back(new mixed(new TFT_predict(E), 80, new always(MAX_BID), 20));
	competitors.push_back(new mixed(new TFT_predict(E), 80, new always(MAX_BID), 10, new always(MIN_BID), 10));
	twins.push_back(new mixed(new TFT_predict(E), 80, new always(MAX_BID), 20));
	twins.push_back(new mixed(new TFT_predict(E), 80, new always(MAX_BID), 10, new always(MIN_BID), 10));

	competitors.push_back(new mixed(new TFT_simple, 80, new random(MAX_BID-GRANULARITY,MAX_BID), 20));
	twins.push_back(new mixed(new TFT_simple, 80, new random(MAX_BID-GRANULARITY,MAX_BID), 20));

	competitors.push_back(new buckets(FW_HIGHEST));
	competitors.push_back(new buckets(FW_LOWEST));
	competitors.push_back(new buckets(FW_RANDOM));
	competitors.push_back(new buckets(FW_NEWEST));
	twins.push_back(new buckets(FW_HIGHEST));
	twins.push_back(new buckets(FW_LOWEST));
	twins.push_back(new buckets(FW_RANDOM));
	twins.push_back(new buckets(FW_NEWEST));

	competitors.push_back(new buckets(PD, 0.8));
	competitors.push_back(new buckets(PD, 0.5));
	competitors.push_back(new buckets(PD, 0.2));
	twins.push_back(new buckets(PD, 0.8));
	twins.push_back(new buckets(PD, 0.5));
	twins.push_back(new buckets(PD, 0.2));

	competitors.push_back(new buckets(FW_HIGHEST, 0, false));
	competitors.push_back(new buckets(FW_RANDOM, 0, false));
	twins.push_back(new buckets(FW_HIGHEST, 0, false));
	twins.push_back(new buckets(FW_RANDOM, 0, false));

	competitors.push_back(new buckets(PD, 0.8, false));
	competitors.push_back(new buckets(PD, 0.5, false));
	competitors.push_back(new buckets(PD, 0.2, false));
	twins.push_back(new buckets(PD, 0.8, false));
	twins.push_back(new buckets(PD, 0.5, false));
	twins.push_back(new buckets(PD, 0.2, false));

	competitors.push_back(new Zeuthen(POS));
	competitors.push_back(new Zeuthen(NEG));
	twins.push_back(new Zeuthen(POS));
	twins.push_back(new Zeuthen(NEG));

	competitors.push_back(new simple_trend(3, 0.5));
	competitors.push_back(new simple_trend(10, 0.5));
	competitors.push_back(new simple_trend(25, 0.5));
	twins.push_back(new simple_trend(3, 0.5));
	twins.push_back(new simple_trend(10, 0.5));
	twins.push_back(new simple_trend(25, 0.5));

	competitors.push_back(new simple_trend_new(3, 0.5));
	competitors.push_back(new simple_trend_new(10, 0.5));
	competitors.push_back(new simple_trend_new(25, 0.5));
	twins.push_back(new simple_trend_new(3, 0.5));
	twins.push_back(new simple_trend_new(10, 0.5));
	twins.push_back(new simple_trend_new(25, 0.5));

	competitors.push_back(new simple_trend_tweak(3, 0.5));
	competitors.push_back(new simple_trend_tweak(10, 0.5));
	competitors.push_back(new simple_trend_tweak(25, 0.5));
	twins.push_back(new simple_trend_tweak(3, 0.5));
	twins.push_back(new simple_trend_tweak(10, 0.5));
	twins.push_back(new simple_trend_tweak(25, 0.5));

	competitors.push_back(new qlearning(0.8, 0.));
	competitors.push_back(new qlearning(0.5, 0.));
	competitors.push_back(new qlearning(0.2, 0.));
	twins.push_back(new qlearning(0.8, 0.));
	twins.push_back(new qlearning(0.5, 0.));
	twins.push_back(new qlearning(0.2, 0.));

	competitors.push_back(new qlearning(0.8, 0.9));
	competitors.push_back(new qlearning(0.5, 0.9));
	competitors.push_back(new qlearning(0.2, 0.9));
	twins.push_back(new qlearning(0.8, 0.9));
	twins.push_back(new qlearning(0.5, 0.9));
	twins.push_back(new qlearning(0.2, 0.9));


	for(unsigned int x=0; x<twins.size(); x++)
		twins[x]->logging = false;
}

void tournament::sortCompetitors(int utility)
{
	//Bubble sort by utility
	for(unsigned int i = 0; i < competitors.size()-1; i++)
		for(unsigned int j = competitors.size()-1; j >= i+1; j--)
			if(competitors[j]->Utils[utility] > competitors[j-1]->Utils[utility])
			{
				traveler* temp = competitors[j];
				competitors[j] = competitors[j-1];
				competitors[j-1] = temp;
			}
}

void tournament::logRanking(int utility)
{
	switch(utility)
	{
	case 0: log << "Ranking based on U1";
		break;
	case 1: log << "Ranking based on U2 for " << competitors.size()*matches_to_play <<" matches. (";
			log << competitors.size() << " opponents, " << matches_to_play << " matches each)";
		break;
	case 2: log << "Ranking based on U3";
		break;
	case 3: log << "Ranking based on U3'";
		break;
	default: log << "Bad log choice in log function call.";
	}

	log << endl << endl;

	for(unsigned int x=0; x < competitors.size(); x++)
		log << competitors[x]->Utils[utility] << ":\t" << competitors[x]->name << endl;

	log << endl << endl;
}

void tournament::TeXlogRanking(int utility)
{
	string utilRef;

	TeXlog << "\\begin{table}[!hbtp]" << endl;
	TeXlog << "\\subsection{Ranking Based on ";

	switch(utility)
	{
	case 0: TeXlog << "U1";
			utilRef = "U1";		
		break;
	case 1: TeXlog << "U2";
			utilRef = "U2";
		break;
	case 2: TeXlog << "U3";
			utilRef = "U3";
		break;
	case 3: TeXlog << "U3'";
			utilRef = "U3p";
		break;
	default: TeXlog << "Bad util choice in TeXlog function call.";
	}

	TeXlog << "}" << endl;

	TeXlog << "\\begin{footnotesize}" << endl;
	TeXlog << "\\begin{tabular}{|r|l|}\\hline  \\label{U3results}" << endl;

	// Fix % for latex	
	string str;
	string searchString( "%" ); 
    string replaceString( "\\%" );

    assert( searchString != replaceString );
	
	for(unsigned int x=0; x < competitors.size(); x++)
	{
		str  = competitors[x]->name;
		string::size_type pos = 0;
		while ( (pos = str.find(searchString, pos)) != string::npos ) {
			str.replace( pos, searchString.size(), replaceString );
			pos+=2;
		}

		TeXlog << competitors[x]->Utils[utility] << " & " << str << "\\\\ \\hline"<<endl;
	}

	TeXlog << "\\end{tabular}" << endl;
	TeXlog << "\\caption{for metric definition see section \\eqref{" << utilRef << "}}" << endl;
	TeXlog << "\\end{footnotesize}" << endl;
	TeXlog << "\\end{table}" << endl << endl;
}

pair<int, int> tournament::getPayoffs(int xBid, int yBid)
{
	pair<int, int> payoffs;

	if(xBid < yBid)
	{
		payoffs.first = xBid + BONUS_MALUS;
		payoffs.second = xBid - BONUS_MALUS;
	}
	else if(yBid < xBid)
	{
		payoffs.first = yBid - BONUS_MALUS;
		payoffs.second = yBid + BONUS_MALUS;
	}
	else
	{
		payoffs.first = xBid;
		payoffs.second = xBid;
	}

	return payoffs;
}

void tournament::printRanking(int utility)
{
	switch(utility)
	{
	case 0: cout << "Ranking based on U1";
		break;
	case 1: cout << "Ranking based on U2 for " << competitors.size()*matches_to_play <<" matches. (";
			cout << competitors.size() << " opponents, " << matches_to_play << " matches each)";
		break;
	case 2: cout << "Ranking based on U3";
		break;
	case 3: cout << "Ranking based on U3'";
		break;
	default: cout << "Bad print choice in print function call.";
	}
	cout << endl << endl;

	for(unsigned int x=0; x < competitors.size(); x++)
		cout << competitors[x]->Utils[utility] << ":\t" << competitors[x]->name << endl;

	cout << endl << endl;
}

void tournament::normalizeUtils()
{
	for(unsigned int x=0; x < competitors.size(); x++)
	{
		competitors[x]->Utils[0]/=((MAX_BID + BONUS_MALUS)*competitors.size()*(double)rounds_to_play*(double)matches_to_play);
		competitors[x]->Utils[1]/=(2.0*competitors.size()*(double)matches_to_play);
		competitors[x]->Utils[1]+= 0.5;
		competitors[x]->Utils[2]/=(competitors.size()*(double)ROUNDS*(double)MATCHES);
		competitors[x]->Utils[3]/=(competitors.size()*(double)ROUNDS*(double)MATCHES);
	}
}

void tournament::printPercentComplete()
{
	if((int)current_match % 50 == 0)
	{
		last_percent = new_percent;
		new_percent = (int)floor(10000.0*current_match/total_matches + 0.5);

		double display_percent = (double)new_percent/100.0;

		if(new_percent != last_percent)
		{	
			//clear screen
			system("cls");  // Makes it pretty, but slows it down A LOT

			cout << "Running..." << endl; 
			cout << "Percent complete: "<< display_percent << "%" << endl;
		}
	}
}