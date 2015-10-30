#include "crawler.h"
#include "strategy.h"
#include <vector>
#include <iostream>
#include <conio.h>

using namespace std;

void main()
{
	crawler C(200);

	C.crawl();

	_getch();
	
}