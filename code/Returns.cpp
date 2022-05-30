#include "Returns.h"
#include <vector>
#include <numeric>
#include <assert.h>
namespace Team_Project
{
	double VectorSum(vector<double>& V)
	{
		double sum = 0.0;
		int d = (int)V.size();
		for (int i = 0; i < V.size(); i++)
		{
			sum = sum + V[i];
		}
		return sum;
	}

	void returns::cal_daily_avgs(vector<double>& ret,vector<double>& ret1) 
	{
		double temp = VectorSum(ret)/ret.size();//accumulate(ret.begin(), ret.end(), 0.0) / ret.size();
		double temp2 = VectorSum(ret1)/ret1.size();
		dailyRet_avg.push_back(temp);
		CAAR_per.push_back(temp2);
		//cout << group;
		//cout << "CALC DAILY RETS is******************************" << endl;
		//cout << dailyRet_avg << endl;
		//cout << "**************************************************" << endl;
	}
	/*
	void returns::calc_CAAR() 
	{
		CAAR_per.resize(dailyRet_avg.size());
		partial_sum(dailyRet_avg.begin(), dailyRet_avg.end(), CAAR_per.begin());
		cout << group;
		cout << "CAAR PER SAMPLE IS %%%%%%%%%%%%%%%%%%%%%%" << endl;
		cout << CAAR_per;
		cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
	}
	*/
	void returns::FinalAverages()
	{
		
		vector<double> temp3;
		vector<double> temp4;
		
		for (int l = 0; l < AAR[0].size(); l++)
		{
			for (int m = 0; m < ss; m++)
			{
				temp3.push_back(AAR[m][l]);
				temp4.push_back(CAAR[m][l]);
			}
			final_arr.push_back(VectorSum(temp3) / temp3.size());
			final_caar.push_back(VectorSum(temp4) / temp4.size());
			temp3.clear();
			temp4.clear();
		}
	}

	void returns::StandardDeviations()
	{
		AARStdDevVector.clear();
		CAARStdDevVector.clear();
		vector<double> temp;
		vector<double> temp2;
		for (int l = 0; l < AAR[0].size(); l++)
		{
			for (int m = 0; m < ss; m++)
			{
				temp.push_back(AAR[m][l]);
				temp2.push_back(CAAR[m][l]);
			}
			double mean = VectorSum(temp) / temp.size();
			double sq_sum = std::inner_product(temp.begin(), temp.end(), temp.begin(), 0.0) / temp.size();
			AARStdDevVector.push_back(sqrt(sq_sum - mean * mean));
			double mean2 = VectorSum(temp2) / temp2.size();
			double sq_sum2 = std::inner_product(temp2.begin(), temp2.end(), temp2.begin(), 0.0) / temp2.size();
			CAARStdDevVector.push_back(sqrt(sq_sum2 - mean2 * mean2));

			temp.clear();
			temp2.clear();
		}
	}

	void returns::populateAAR(int sample_number)
	{
		for (int i = 0; i < dailyRet_avg.size(); i++)
		{
			AAR[sample_number].push_back(dailyRet_avg[i]);
		}
		dailyRet_avg.clear();
	}

	void returns::populateCAAR(int sample_number)
	{
		for (int i = 0; i < CAAR_per.size(); i++)
		{
			CAAR[sample_number].push_back(CAAR_per[i]);
		}
		CAAR_per.clear();
	}

	void returns::ClearDataforNewCalc()//int n)
	{
		final_arr.clear();
		final_caar.clear();
		AAR.clear();
		CAAR.clear();
		AARStdDevVector.clear();
		CAARStdDevVector.clear();
		SetSize();


	}
}
