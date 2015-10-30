#pragma once

#include <vector>

#include "strategy.h"

using namespace std;

typedef enum strats {
	RAND_ALL,
	RAND_TOP,	
	ALWAYS_2,
	ALWAYS_51,
	ALWAYS_99,
	ALWAYS_100,
	BUCKET_HIGH,
	BUCKET_LOW,
	BUCKET_RAND,
	BUCKET_NEW,
	BUCKET_02,
	BUCKET_05,
	BUCKET_08,
	QLEARN_02_0,
	QLEARN_05_0,
	QLEARN_08_0,
	QLEARN_02_9,
	QLEARN_05_9,
	QLEARN_08_9,
	ST_3,
	ST_10,
	ST_25,
	Mixed_1, //Mixed_{TFT (y-10), 80%); (R[90, 100], 20%)},
	Mixed_2, //Mixed_{L(x) E(x) H(y-g), 80%); (20, 20%)},
	
	Mixed_3, //Mixed_{L(x) E(x) H(y-g), 80%); (100, 20%)},
	Mixed_4, //Mixed_{L(x) E(x) H(y-g), 80%); (100, 10%); (20, 10%)},
	
	Mixed_5, //Mixed_{L(y-g) E(x-g) H(x-g), 80%); (20, 20%)},
	Mixed_6, //Mixed_{L(y-g) E(x-g) H(x-g), 80%); (100, 20%)},
	Mixed_7, //Mixed_{L(y-g) E(x-g) H(x-g), 80%); (100, 10%); (20, 10%)},
	
	TFT1, //TFT_Low(x) Equal(x)    High(y-g),
	TFT2, //TFT_Low(x) Equal(x-2g) High(y-g),

	TFT3, //TFT_Low(y-g) Equal(x-g) High(x-g),

	TFT4, //TFT_Low(x-2g) Equal(x)    High(y-g),
	TFT5, //TFT_Low(x-2g) Equal(x-2g) High(y-g),

	TFT_SIMPLE_1,
	TFT_SIMPLE_2,
	ZEUTHEN_P,
	ZEUTHEN_N,
	STRAT_SIZE
} strats;

class crawler
{
public:
	crawler(void);
	~crawler(void);

	crawler(int size);

	void crawl();

	vector<strategy *> C; 

};

