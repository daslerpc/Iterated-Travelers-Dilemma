#include "crawler.h"
#include <assert.h>
#include <iostream>


#include <windows.h>

crawler::crawler(void)
{
}

crawler::crawler(int EpochSize)
{
	assert(1000%EpochSize==0);

/*	for(int x=0; x < 2; x++)
		C.push_back(new strategy(EpochSize));

	C[ALWAYS_2]->setName("Always 2");
	C[ALWAYS_51]->setName("Always 51");
*/

	for(int x=0; x < STRAT_SIZE; x++)
		C.push_back(new strategy(EpochSize));

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
	C[BUCKET_10]->setName("Buckets - PD, Retention = 1.0");

	C[TFT_SIMPLE_1]->setName("TFT - Simple (y-1)");
	C[TFT_SIMPLE_2]->setName("TFT - Simple (y-2)");

	C[TFT1]->setName("TFT - Low(x) Equal(x) High(y-g)");
	C[TFT2]->setName("TFT - Low(x) Equal(x-2g) High(y-g)");
	C[TFT3]->setName("TFT - Low(y-g) Equal(x-g) High(x-g)");
	C[TFT4]->setName("TFT - Low(x-2g) Equal(x) High(y-g)");
	C[TFT5]->setName("TFT - Low(x-2g) Equal(x-2g) High(y-g)");
	
	C[QLEARN_02]->setName("Q Learn - alpha= 0.2, discount= 0.5");
	C[QLEARN_05]->setName("Q Learn - alpha= 0.5, discount= 0.5");
	C[QLEARN_08]->setName("Q Learn - alpha= 0.8, discount= 0.5");

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
	for(unsigned int x=0; x < C.size()-1; x++)
		for(unsigned int y=x+1; y < C.size(); y++)
		{
			C[x]->getScores(C[y]->name);
			C[y]->getScores(C[x]->name);
		}

	for(unsigned int x=0; x < C.size(); x++)
		C[x]->printScores();

	ofstream out;
	string filename = "Epoch_Logs/";

	CreateDirectory(filename.c_str(), NULL);

	filename.append("all_results.txt");
	out.open(filename.c_str());

	for(unsigned int x=0; x < C.size(); x++)
		out << C[x]->name << "\t";

	for(unsigned int y=0; y < C[0]->epoch_num; y++)
	{
		out << endl;

		for(unsigned int x=0; x < C.size(); x++)
			out << C[x]->scores[y] << "\t";
	}

	for(unsigned int x=0; x < C.size(); x++)
		C[x]->logScores();


}

crawler::~crawler(void)
{
}
