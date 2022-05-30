#include <iostream>
#include <vector>
#include <iomanip>
#include"Bootstrap.h"
#include <fstream>
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <time.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <locale.h>
#include <chrono>
#include <thread>
#include "curl/curl.h"
#include"Matrix.h"
#include <windows.h>

string validatestring(string t)
{
	string t2;
	if (t.back() != '\0') t.push_back('\0');
	char c; int i = 0;
	while (t[i] != '\0')
	{
		c = t[i];
		if (islower(c)) c = toupper(c);
		t2.push_back(c);
		i++;
	}
	return t2;
}



using namespace std;
using namespace Team_Project;


#define NUM_THREADS 3
int main()
{
	curl_global_init(CURL_GLOBAL_ALL);
	std::chrono::time_point<std::chrono::system_clock> start, end;
	string filename = "Russell3000EarningsAnnouncements.csv";
	string startdate = "2020-01-01";
	string enddate = "2022-05-10";
	BootStrapp Boot(startdate,enddate);
	returns rBeat(40, "Beat");
	returns rSame(40, "Meet");
	returns rMiss(40, "Miss");
	//cout << "Starting to Retrieve Index Data: " << endl;
	Boot.getIVWData();
	//cout << "Retreived Index Data: " << endl;
	int N = 0;
	int input = 0;
	string s;
	string tick;
	int Group;
	while (input != 5)
	{
		cout << "Menu" << endl;
		cout << "Enter 1 to Retrieve price data for all stocks " << endl;
		cout << "Enter 2 to Pull Individual Stock Data" << endl;
		cout << "Enter 3 to Show Group Data" << endl;
		cout << "Enter 4 to Plot Returns" << endl;
		cout << "Enter 5 to Exit: " << endl;

		cin >> input;
		switch(input)
		{
		case 1:
			if (Boot.MyMap.empty())
			{
				using namespace std::chrono;
				
				//cout << "Starting the create the stock Map from the excel file" << endl;
				Boot.createStockMap(filename);
				//Boot.printstockmap();
				//cout << "Done creating Map" << endl;
				

			}
			cout << "Enter the Number of the days for which the stock Data Needs to be populated" << endl;
			cin >> N;
			if (N < 60 || N>90)
			{
				cout << "The days to retrieve data should be atleast 60 but less than 90" << endl;
				break;
			}
			else if (Boot.GetN() == N)
			{
				cout << "You already have the data for the entered days" << endl;
				break;
			}
			else
			{
				Boot.SetN(N);
				start = std::chrono::system_clock::now();
				cout << "Now starting to Populate the stock map with prices: " << endl;
				Boot.PopulateMultiThread();
				end = std::chrono::system_clock::now();
				std::chrono::duration<double> elapsed_seconds = end - start;
				std::cout << "Data Retrieval elapsed time: " << elapsed_seconds.count() << "s\n";
				rBeat.ClearDataforNewCalc();
				rSame.ClearDataforNewCalc();
				rMiss.ClearDataforNewCalc();
				Boot.Cal_Groups(&rBeat, &rSame, &rMiss);

				std::time_t end_time = std::chrono::system_clock::to_time_t(end);
				std::cout << "finished computation at " << std::ctime(&end_time) << endl;

				cout << "The number of Stocks in Beat Group is : " << Boot.MyMapBeat.size() << endl;

				cout << "The number of Stocks in Meet Group is : " << Boot.MyMapSame.size() << endl;

				cout << "The number of Stocks in Miss Group is : " << Boot.MyMapMiss.size() << endl;

				break;
			}
		case 2:
			if (Boot.MyMap.empty())
			{
				cout << "The Map has not yet been populated for the first time, please select option 1 to create and populate stock map first:" << endl;
				break;
			}

			cout << "Enter the stock ticker to pull out the data:" << endl;
			cin >> tick;

			s = validatestring(tick);
			Boot.MyMap[s]->pullinformation();
			break;
		case 3:
			if (Boot.MyMap.empty())
			{
				cout << "The Map has not yet been populated for the first time, please select option 1 to create and populate stock map first:" << endl;
				break;
			}
			
			cout << "Enter 1 to see Beat group : " << endl;
			cout << "Enter 2 to see Meet group : " << endl;
			cout << "Enter 3 to see Miss group : " << endl;
			cin >> Group;
			switch (Group) 
			{
			case 1:
				cout << "*********** Beat Group **************" << endl;
				cout << "AAR for Beat Group is : " << endl;
				cout << rBeat.final_arr << endl;
				cout << "AAR STD for Beat Group is : " << endl;
				cout << rBeat.AARStdDevVector << endl;
				cout << "CAAR for Beat Group is : " << endl;
				cout << rBeat.final_caar << endl;
				cout << "CAAR STD for Beat Group is : " << endl;
				cout << rBeat.CAARStdDevVector << endl;
				break;
		case 2:
				cout << "*********** Meet Group **************" << endl;
				cout << "AAR for Meet Group is : " << endl;
				cout << rSame.final_arr << endl;
				cout << "AAR STD for Meet Group is : " << endl;
				cout << rSame.AARStdDevVector << endl;
				cout << "CAAR for Meet Group is : " << endl;
				cout << rSame.final_caar << endl;
				cout << "CAAR STD for Meet Group is : " << endl;
				cout << rSame.CAARStdDevVector << endl;
				break;
			case 3:
				cout << "*********** Miss Group **************" << endl;
				cout << "AAR for Miss Group is :" << endl;
				cout << rMiss.final_arr << endl;
				cout << "AAR STD for Miss Group is :" << endl;
				cout << rMiss.AARSTDDEV << endl;
				cout << "AAR for Miss Group is :" << endl;
				cout << rMiss.final_caar << endl;
				cout << "CAAR STD for Miss Group is :" << endl;
				cout << rMiss.CAARStdDevVector << endl;
				break;
			}
			break;
		case 4:
			if (rBeat.get_CAAR().empty())
			{
				cout << "Returns are not yet calculated:" << endl;
				break;
			}
			Boot.plotreturns(N, &rBeat, &rSame, &rMiss);
			break;
		case 5:
			break;
		default:
			cout << "Please Enter a valid input:" << endl;
			break;
		}
	}

	
	
		

	curl_global_cleanup();

	
	return 0;
}
