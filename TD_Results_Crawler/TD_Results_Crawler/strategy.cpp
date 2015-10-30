#include "strategy.h"

#include <iostream>
#include <stdlib.h>

strategy::strategy(void)
{
}

strategy::strategy(int Epoch_Size)
{
	epoch_size = Epoch_Size;
	epoch_num = 1000/epoch_size;
	for(int x=0; x < epoch_num; x++)
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

	input.open(filename.c_str());
	double this_score = 0;

	if (input.is_open())
	{
		char buffer[256];

		for(int match_cnt=0; match_cnt < 100; match_cnt++)
		{
			for(int cnt=0; cnt<3; cnt++)
				input.getline(buffer, 256); // clear titles and spacing

			for(int epoch_cnt=0; epoch_cnt < epoch_num; epoch_cnt++)
			{
				for(int round_cnt=0; round_cnt < epoch_size; round_cnt++)
				{
					input.getline(buffer, 256);  // get the next action/reward line

					char * pch;
					pch = strtok (buffer,"()"); // clear the action pair
					pch = strtok (NULL, "()"); // get the score
					
					this_score = atof(pch);  // turn string into double

					scores[epoch_cnt] += this_score;  // add score to appropriate epoch sum
				}
			}
		}

		// normalize scores: divide by matches X epoch_size X max score (i.e. total rounds x max score)
		for(int epoch_cnt=0; epoch_cnt < epoch_num; epoch_cnt++)  
			scores[epoch_cnt] /= (double)(100 * 200 * 101);  
		
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
	for(unsigned int x=0; x < scores.size(); x++)
		cout << "Epoch " << x+1 << ":\t" << scores[x] << endl;
	cout << "***************************\n\n";
}

void strategy::logScores()
{
	ofstream out;
	string filename = "Epoch_Logs/";

	filename.append(name);
	filename.append(".txt");
	out.open(filename.c_str());

	for(unsigned int x=0; x < scores.size(); x++)
		out << scores[x] << endl;
}