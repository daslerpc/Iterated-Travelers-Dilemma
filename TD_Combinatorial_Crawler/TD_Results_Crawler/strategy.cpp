#include "strategy.h"

#include <iostream>
#include <stdlib.h>
#include <conio.h>

strategy::strategy()
{
	for(int x=0; x < NUM_GROUPS; x++)
		scores.push_back(0);
}

strategy::~strategy(void)
{
}

void strategy::setName(string inName)
{
	name = inName;
}


void strategy::getScores(string opp_name)
{
	ifstream input;
	string filename = "logs/";
	filename.append(name);
	filename.append("/");
	filename.append(name);
	filename.append(" vs ");
	filename.append(opp_name);
	filename.append(".txt");

	int index;

	if(opp_name == "Twin")
		opp_name = name;

	switch( opp_name[0] ) {
		case 'R': index = RAND;
			break;
		case 'A': index = ALWAYS;
			break;
		case 'B': 
				if(opp_name[10] == 'P')
					index = BUCKET_PROB;
				else if(opp_name[10] == '(')
					index = BUCKET_DET;
				else
				{
					cout << "ERROR!  Bucket strategy class not recognized" << endl;
					_getch();
				}
			break;
		case 'Q': 
				if(opp_name[34] == '0')
					index = QLEARN_SHORT;
				else if(opp_name[34] == '9')
					index = QLEARN_LONG;
				else
				{
					cout << "ERROR!  Q-Learning strategy class not recognized" << endl;
					_getch();
				}
			break;
		case 'S': index = SIMPLE_TREND;
			break;
		case 'M': 
				if(opp_name[9] == 'T')
					index = TFT_MIXED;
				else if(opp_name[9] == 'L')
					index = MIXED;
				else
				{
					cout << "ERROR!  Mixed strategy class not recognized" << endl;
					_getch();
				}
			break;
		case 'T': 
				if(opp_name[6] == 'S')
					index = TFT_SIMPLE;
				else if(opp_name[6] == 'L')
					index = TFT_COMPLEX;
				else
				{
					cout << "ERROR!  TFT strategy class not recognized" << endl;
					_getch();
				}
			break;
		case 'Z': index = ZEUTHEN;
			break;
		default: cout << "ERROR!  Strategy class not recognized" << endl;
		_getch();
	}
	
	input.open(filename.c_str());
	double this_score = 0;

	if (input.is_open())
	{
		char buffer[256];

		for(int match_cnt=0; match_cnt < 100; match_cnt++)
		{
			for(int cnt=0; cnt<3; cnt++)
				input.getline(buffer, 256); // clear titles and spacing

			for(int round_cnt=0; round_cnt < 1000; round_cnt++)
			{
				input.getline(buffer, 256);  // get the next action/reward line

				char * pch;
				pch = strtok (buffer,"()"); // clear the action pair
				pch = strtok (NULL, "()"); // get the score
					
				this_score = atof(pch);  // turn string into double

				scores[index] += this_score;  // add score to appropriate epoch sum
			}
			
		}

		// normalize scores: divide by matches X epoch_size X max score (i.e. total rounds x max score)
		//for(int epoch_cnt=0; epoch_cnt < 8; epoch_cnt++)  
		//	scores[epoch_cnt] /= (double)(100 * 1000 * 101);  
		
	}
	else
	{
		cout << "Failed to open file: " << filename << endl;
	}
}

void strategy::printScores()
{
	cout << "Strategy:\t" << name << endl;
	cout << "***************************\n";
	
	cout << "Randoms:\t" << scores[RAND] << endl;
	cout << "Always:\t" << scores[ALWAYS] << endl;
	cout << "Buckets - Prob:\t" << scores[BUCKET_PROB] << endl;
	cout << "Buckets - Det:\t" << scores[BUCKET_DET] << endl;
	cout << "Q Learners - short:\t" << scores[QLEARN_SHORT] << endl;
	cout << "Q Learners - long:\t" << scores[QLEARN_LONG] << endl;
	cout << "Simple Trends:\t" << scores[SIMPLE_TREND] << endl;
	cout << "Mixed:\t" << scores[MIXED] << endl;
	cout << "TFTs - simple:\t" << scores[TFT_SIMPLE] << endl;
	cout << "TFTs - complex:\t" << scores[TFT_COMPLEX] << endl;
	cout << "TFTs - mixed:\t" << scores[TFT_MIXED] << endl;
	cout << "Zeuthens:\t" << scores[ZEUTHEN] << endl;

	cout << "***************************\n\n";
}

void strategy::logScores()
{
	ofstream out;
	string filename = "Epoch_Logs/";

	filename.append(name);
	filename.append(".txt");
	out.open(filename.c_str());

	out << "Randoms:\t" << scores[RAND] << endl;
	out << "Always:\t" << scores[ALWAYS] << endl;
	out << "Buckets - Prob:\t" << scores[BUCKET_PROB] << endl;
	out << "Buckets - Det:\t" << scores[BUCKET_DET] << endl;
	out << "Q Learners - short:\t" << scores[QLEARN_SHORT] << endl;
	out << "Q Learners - long:\t" << scores[QLEARN_LONG] << endl;
	out << "Simple Trends:\t" << scores[SIMPLE_TREND] << endl;
	out << "Mixed:\t" << scores[MIXED] << endl;
	out << "TFTs - simple:\t" << scores[TFT_SIMPLE] << endl;
	out << "TFTs - complex:\t" << scores[TFT_COMPLEX] << endl;
	out << "TFTs - mixed:\t" << scores[TFT_MIXED] << endl;
	out << "Zeuthens:\t" << scores[ZEUTHEN] << endl;
}