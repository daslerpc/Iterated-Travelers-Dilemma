#include <iostream> // For cout
#include <conio.h>  // For _gethch
#include <time.h>	// For timing analysis

#include <fstream> // For file logging
#include <direct.h> // to create folders
#include <math.h>

#include "tournament.h"

#include "always.h"

using namespace std;

const int MIN_BID = 2;
const int MAX_BID = 100;
const int GRANULARITY = 1;
const int BONUS_MALUS = 2;

const int MATCHES = 100;
const int ROUNDS = 1000;

ofstream error;
ofstream longnames;
ofstream shortnames;

string params;

void setParamString();

void main()
{
	CreateDirectory("logs/", NULL);

	setParamString();

	string filename = "logs/";

	CreateDirectory(filename.c_str(), NULL);

	filename.append("Error log.txt");
	error.open(filename.c_str());

	clock_t start_time = clock();

	cout << "Initializing tournament." << endl;

	tournament T;
	
	// Add strategies to the competitors list
	T.addCompetitors();

	//Run the tournament
	T.runTournament(MATCHES, ROUNDS);

	clock_t elapsed = clock() - start_time;
	
	int min = (int)floor((1000.0*elapsed/CLOCKS_PER_SEC)/60000.0);
	double sec = (1000.0*elapsed/CLOCKS_PER_SEC)/1000.0 - min*60.0;

	cout << endl << endl << "Execution time: " << endl;
	cout << min << " minute(s), " << sec << " seconds." << endl << endl;

	longnames.open("Long_Names.txt");
	shortnames.open("Short_Names.txt");

	for(int x=0; x<T.competitors.size(); x++)
	{
		longnames << T.competitors[x]->name << endl;
		shortnames << T.competitors[x]->short_name << endl;
	}
	
	cout << "Press any key to continue.";
	_getch();

	error.close();
}

void setParamString()
{
	params = "(";

	char temp[4];	// buffer to hold "always" string below

	_itoa_s (MIN_BID, temp, 10);	// change the value to a string
	params.append(temp);

	params.append("-");

	_itoa_s (MAX_BID, temp, 10);	// change the value to a string
	params.append(temp);

	params.append("), g=");
	
	_itoa_s (GRANULARITY, temp, 10);	// change the value to a string
	params.append(temp);

	params.append(", d=");

	
	_itoa_s (BONUS_MALUS, temp, 10);	// change the value to a string
	params.append(temp);
}