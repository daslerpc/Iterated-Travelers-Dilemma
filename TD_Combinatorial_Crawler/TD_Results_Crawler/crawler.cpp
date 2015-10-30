#include "crawler.h"
#include <assert.h>
#include <iostream>


#include <windows.h>


crawler::crawler()
{

/*	for(int x=0; x < 2; x++)
		C.push_back(new strategy(EpochSize));

	C[ALWAYS_2]->setName("Always 2");
	C[ALWAYS_51]->setName("Always 51");
*/

	for(int x=0; x < STRAT_SIZE; x++)
		C.push_back(new strategy());

	C[RAND_ALL]->setName("Random [2, 100]");
	C[RAND_TOP]->setName("Random [99, 100]");
	
	C[ALWAYS_2]->setName("Always 2");
	C[ALWAYS_51]->setName("Always 51");
	C[ALWAYS_99]->setName("Always 99");
	C[ALWAYS_100]->setName("Always 100");
	
	C[BUCKET_LOW]->setName("Buckets - (Fullest, Lowest)");
	C[BUCKET_HIGH]->setName("Buckets - (Fullest, Highest)");
	C[BUCKET_NEW]->setName("Buckets - (Fullest, Newest)");
	C[BUCKET_RAND]->setName("Buckets - (Fullest, Random)");

	C[BUCKET_02]->setName("Buckets - PD, Retention = 0.2");
	C[BUCKET_05]->setName("Buckets - PD, Retention = 0.5");
	C[BUCKET_08]->setName("Buckets - PD, Retention = 0.8");

	C[TFT_SIMPLE_1]->setName("TFT - Simple (y-1)");
	C[TFT_SIMPLE_2]->setName("TFT - Simple (y-2)");

	C[TFT1]->setName("TFT - Low(x) Equal(x) High(y-g)");
	C[TFT2]->setName("TFT - Low(x) Equal(x-2g) High(y-g)");
	C[TFT3]->setName("TFT - Low(y-g) Equal(x-g) High(x-g)");
	C[TFT4]->setName("TFT - Low(x-2g) Equal(x) High(y-g)");
	C[TFT5]->setName("TFT - Low(x-2g) Equal(x-2g) High(y-g)");
	
	C[QLEARN_02_0]->setName("Q Learn - alpha= 0.2, discount= 0.0");
	C[QLEARN_05_0]->setName("Q Learn - alpha= 0.5, discount= 0.0");
	C[QLEARN_08_0]->setName("Q Learn - alpha= 0.8, discount= 0.0");
	
	C[QLEARN_02_9]->setName("Q Learn - alpha= 0.2, discount= 0.9");
	C[QLEARN_05_9]->setName("Q Learn - alpha= 0.5, discount= 0.9");
	C[QLEARN_08_9]->setName("Q Learn - alpha= 0.8, discount= 0.9");

	C[ST_3]->setName("Simple Trend - K = 3, Eps = 0.5");
	C[ST_10]->setName("Simple Trend - K = 10, Eps = 0.5");
	C[ST_25]->setName("Simple Trend - K = 25, Eps = 0.5");

	
	C[Mixed_1]->setName("Mixed - {TFT (y-1), 80%); (R[99, 100], 20%)}");
	
	C[Mixed_2]->setName("Mixed - {L(x) E(x) H(y-g), 80%); (2, 20%)}");
	C[Mixed_3]->setName("Mixed - {L(x) E(x) H(y-g), 80%); (100, 20%)}");
	C[Mixed_4]->setName("Mixed - {L(x) E(x) H(y-g), 80%); (100, 10%); (2, 10%)}");
	
	C[Mixed_5]->setName("Mixed - {L(y-g) E(x-g) H(x-g), 80%); (2, 20%)}");
	C[Mixed_6]->setName("Mixed - {L(y-g) E(x-g) H(x-g), 80%); (100, 20%)}");
	C[Mixed_7]->setName("Mixed - {L(y-g) E(x-g) H(x-g), 80%); (100, 10%); (2, 10%)}");
	
	C[ZEUTHEN_P]->setName("Zeuthen Strategy - Positive");
	C[ZEUTHEN_N]->setName("Zeuthen Strategy - Negative");
	
}


void crawler::crawl()
{
	double total = 36 * 36;
	double current = 0;

	for(unsigned int x=0; x < C.size(); x++)
	{
		for(unsigned int y=x; y < C.size(); y++)
		{
			if(x==y)
				C[x]->getScores("Twin");
			else
			{
				C[x]->getScores(C[y]->name);
				C[y]->getScores(C[x]->name);
			}

			current++;
			cout << 100*current/total << "% complete" << endl;
		}
	}

	for(unsigned int x=0; x < C.size(); x++)
		C[x]->printScores();

	ofstream out;
	string filename = "Combinatorial_Logs/";

	CreateDirectory(filename.c_str(), NULL);

	filename.append("all_results.txt");
	out.open(filename.c_str());

	out << "\t";

	for(unsigned int x=0; x < C.size(); x++)
		out << C[x]->name << "\t";

	for(unsigned int y=0; y < 12; y++)
	{
		out << endl;

		switch( y ) 
		{
		case 0: out << "Randoms:\t";
			break;
		case 1: out << "Always:\t";
			break;
		case 2: out << "Buckets - Probabilistic:\t";
			break;
		case 3: out << "Buckets - Deterministice:\t";
			break;
		case 4: out << "Q Learners - Short:\t";
			break;
		case 5: out << "Q Learners - Long:\t";
			break;
		case 6: out << "Simple Trends:\t";
			break;
		case 7: out << "Mixed:\t";
			break;
		case 8: out << "TFTs - Simple:\t";
			break;
		case 9: out << "TFTs - Complex:\t";
			break;
		case 10: out << "TFTs - Mixed:\t";
			break;
		case 11: out << "Zeuthens:\t";
			break;
		}

		for(unsigned int x=0; x < C.size(); x++)
			out << C[x]->scores[y] << "\t";
	}

	for(unsigned int x=0; x < C.size(); x++)
		C[x]->logScores();


}

crawler::~crawler(void)
{
}
