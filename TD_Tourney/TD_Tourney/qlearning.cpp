#include "qlearning.h"

#include <iostream>

qlearning::qlearning(double learning_rate, double discount) : history_size(5), q_init(50.5)
{
	logging = true;
	
	// Set up sizes. (s1 X s2 X s3 X s4 X s5 X a)
	Q.resize(COMP_TYPE_SIZE);
	for (int i = 0; i < COMP_TYPE_SIZE; ++i) 
	{
		Q[i].resize(COMP_TYPE_SIZE);
		for (int j = 0; j < COMP_TYPE_SIZE; ++j)
		{
			Q[i][j].resize(COMP_TYPE_SIZE);
			for (int k = 0; k < COMP_TYPE_SIZE; ++k)
			{
				Q[i][j][k].resize(COMP_TYPE_SIZE);
				for (int l = 0; l < COMP_TYPE_SIZE; ++l)
				{
					Q[i][j][k][l].resize(COMP_TYPE_SIZE);
					for (int m = 0; m < COMP_TYPE_SIZE; ++m)
					{
						Q[i][j][k][l][m].resize(COMP_TYPE_SIZE, q_init);
					}
				}
			}
		}
	}

	history.resize(history_size, LOW);

	not_primed = history_size;


	char temp[32];	// buffer to hold k string below

	name = "Q Learn - alpha= ";
	
	int dummy = (int)learning_rate;
	double temp_double = learning_rate;
	
	_itoa_s (dummy, temp, 10);	// change the learning rate value to a string
	name.append(temp);

	temp_double-=dummy;

	name.append(".");
	temp_double*=10;
	dummy = (int)temp_double;
	
	do
	{
		_itoa_s (dummy, temp, 10);	// change the learning value to a string
		name.append(temp);

		temp_double-=dummy;

		temp_double*=10;
		dummy = (int)temp_double;
	}
	while(temp_double!=0);

	name.append(", discount= ");

	dummy = (int)discount;
	temp_double = discount;
	
	_itoa_s (dummy, temp, 10);	// change the epsilon value to a string
	name.append(temp);

	temp_double-=dummy;

	name.append(".");
	temp_double*=10;
	dummy = (int)temp_double;
	
	do
	{
		_itoa_s (dummy, temp, 10);	// change the epsilon value to a string
		name.append(temp);

		temp_double-=dummy;

		temp_double*=10;
		dummy = (int)temp_double;
	}
	while(temp_double!=0);
}

qlearning::qlearning(void) : history_size(5), q_init(50.5)
{
	name = "Q-Learning Uninitialized";
}


qlearning::~qlearning(void)
{
}

void qlearning::return_Round_Results_Extended(int settlement, int opponents_bid)
{
	comparison action;
	int last_index = history.size()-1;

	if(opponents_bid < my_bid)
		action = HIGH;
	else if(opponents_bid == my_bid)
		action = EQUAL;
	else
		action = LOW;
	
	double past = Q[history[0]][history[1]][history[2]][history[3]][history[4]][action];

	for(int x=0; x < last_index; x++)
		history[x] = history[x+1];

	if(opponents_bid < my_bid)
		history[last_index] = LOW;
	else if(opponents_bid == my_bid)
		history[last_index] = EQUAL;
	else
		history[last_index] = HIGH;
	
		
	if(not_primed)
		not_primed--;
	else
	{
		double present =  settlement;
		double future = discount * bestActionReward();
		Q[history[0]][history[1]][history[2]][history[3]][history[4]][action] = (1.0 - learning_rate) * past + learning_rate * (present + future);
	}
}

int qlearning::getBid_Extended()
{
	if(not_primed)
		my_bid = randomBid();
	else
	{
		double best_reward = 0;
		double predicted_reward = 0;
		int action;

		for(int x = 0; x < COMP_TYPE_SIZE; x++)
		{
			predicted_reward = Q[history[0]][history[1]][history[2]][history[3]][history[4]][x];
			if(predicted_reward > best_reward)
			{
				best_reward = predicted_reward;
				action = x;
			}
		}

		if(action == LOW)
			my_bid = opp_bid - GRANULARITY;
		else if(action == EQUAL)
			my_bid = opp_bid;
		else
			my_bid = opp_bid + GRANULARITY;		
	}

	return my_bid;
}

void qlearning::end_Match_Extended()
{
	for (int i = 0; i < COMP_TYPE_SIZE; ++i) 
	{
		for (int j = 0; j < COMP_TYPE_SIZE; ++j)
		{
			for (int k = 0; k < COMP_TYPE_SIZE; ++k)
			{
				for (int l = 0; l < COMP_TYPE_SIZE; ++l)
				{
					for (int m = 0; m < COMP_TYPE_SIZE; ++m)
					{
						for (int n = 0; n < COMP_TYPE_SIZE; ++n)
						{
							Q[i][j][k][l][m][n] = q_init;
						}
					}
				}
			}
		}
	}

	my_bid = 0;
	opp_bid = 0;

	not_primed = history_size;
}

double qlearning::bestActionReward()
{
	double best_reward = 0;
	double predicted_reward = 0;

	for(int x = 0; x < COMP_TYPE_SIZE; x++)
	{
		predicted_reward = Q[history[0]][history[1]][history[2]][history[3]][history[4]][x];
		if(predicted_reward > best_reward)
			best_reward = predicted_reward;
	}

	return best_reward;
}