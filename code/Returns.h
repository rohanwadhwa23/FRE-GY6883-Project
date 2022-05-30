#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <ctime>
#include"Matrix.h"
#include <numeric>

namespace Team_Project {
	class returns {
	private:
		vector<double> dailyRet_avg;
		vector< vector<double> > AAR;
		vector< vector< double> > CAAR;
		string group;
		int ss;
	public:
		double AARSTDDEV;
		vector<double> CAAR_per;
		vector<double> arr_avg;
		vector<double> CAAR_avg;
		vector<double> final_arr;
		vector<double> final_caar;

		vector<double> AARStdDevVector;
		vector<double> CAARStdDevVector;


		void StandardDeviations();

		returns(int sample_size,  string group_)
		{
			ss = sample_size;
			group = group_;
		}

		void SetSize()
		{
			AAR.resize(ss);//vector<double>(n, 0));
			CAAR.resize(ss);//, vector<double>(n, 0));
		}
		void cal_daily_avgs(vector<double>& ret, vector<double>& ret2);
		void FinalAverages();
		void populateAAR(int sample_number);
		void populateCAAR(int sample_number);

		vector<vector<double>>& get_CAAR()
		{
			return CAAR;
		}

		double RetreiveDayCaar(int number)
		{
			return final_caar[number-1];
		}

		void ClearDataforNewCalc();// int n);

		string getgroupp() { return group; }

		vector<double> &GETDAILY() { return dailyRet_avg; }

	};
}
