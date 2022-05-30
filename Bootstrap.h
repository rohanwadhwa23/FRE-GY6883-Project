#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<cmath>
#include "curl/curl.h"
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include "Ticker.h"
#include <map>
#include<thread>
#include<mutex>
#include"Returns.h"
using namespace std;
namespace Team_Project {
	class BootStrapp {
	private:
		string Startdate;
		string Enddate;
		int N;
		
	public:
		map<string, Stock* > MyMap;
		vector<pair<string, double> > surprise;
		map<string, Stock*> MyMapBeat;
		map<string, Stock*> MyMapSame;
		map<string, Stock*> MyMapMiss;
		vector<string> biglistdatesIVW;
		vector<vector<vector<double>>> ReturnMatrix;

		Index Russel;
		//size_t total;

		BootStrapp(string Startdate_, string Enddate_)
		{
			Startdate = Startdate_;
			Enddate = Enddate_;
			Russel.SetStartDate(Startdate);
			Russel.SetEndDate(Enddate);
		}
		string getStartdate()
		{
			return Startdate;
		}
		string getEnddate()
		{
			return Enddate;
		}
		void Grouping();
		void PopulateMatrix(int i, returns* r);
		void getIVWData();
		void createStockMap(string filename);
		void PopulateMultiThread();
		void PopulateStockData(map<string,Stock*> mp);
		void SetN(int N_) { N = N_; }
		int GetN() { return N; }
		void CaluclateAbnormalReturns();
		void Calc(returns* r);
		void Cal_Groups(returns* r1, returns* r2, returns* r3);
		void generate_random(int sample_size, vector<int>& sample);
		void plotreturns(int nn, returns *r1, returns *r2, returns *r3);;
		void printstockmap()
		{
			string ss("Miss");
			for (auto itr= MyMap.begin(); itr != MyMap.end();itr++)
			{
				string s1(itr->second->groupName());
				if (ss.compare(s1)==0) {
					cout << itr->first << endl;
					cout << itr->second->groupName() << endl;
				}
			}
		}
	};
}
