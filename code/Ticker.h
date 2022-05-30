#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <ctime>
#include"Matrix.h"

using namespace std;

//void convertDateformat(string )
namespace Team_Project {

	class Ticker {
	protected:
		string ticker;
		string StartDate;
		string EndDate;
		vector<double> Price;
		vector<double> Returns;

	public:
		void SetStartDate(string Start) { StartDate = Start; }
		void SetEndDate(string End) { StartDate = End; }
		string GetStartDate() { return StartDate; }
		string GetEndDate() { return EndDate; }

		vector<double> GetPrice() { return Price; }
		vector<double> GetReturns() { return Returns; }

		string get_ticker_id() { return ticker; }

		int returnpricesize() { return Price.size(); }
		void CalculateReturns();

		void setprices(vector<double> prices1);

		void getPrice();

	};

	class Index : public Ticker {
	private:
		string ticker = "IVW";
	public:
		vector<string> dates;

		void SetDates(vector<string> dates1)
		{
			dates.resize(dates1.size());
			for (size_t i = 0; i < dates.size(); i++)
			{
				dates[i] = dates1[i];
			}
		}
	};

	class Stock : public Ticker {
	private:
		string announcementday;
		string Estimate;
		string Reported;
		string Surprise;
		string SurpriseRatio;
		string group;
		vector<double> AbnormalReturns;
		vector<double> CummulativeAbnormalReturns;
	public:
		string aday() { return announcementday; }
		string Estimated_Earning() { return Estimate; }
		string Reported_Earning() { return Reported; }
		string Surprise_Ratio() { return SurpriseRatio; }
		string groupName() { return group; }

		void setgroup(string group_) { group = group_; }

		Stock(string ticker_, string adate_, vector <string> dateslist, string Estimate_, string Reported_, string Surprise_, string SurpriseRatio_)
		{
			ticker = ticker_;
			Estimate = Estimate_;
			Reported = Reported_;
			Surprise = Surprise_;
			SurpriseRatio = SurpriseRatio_;
			//Price.resize(2 * N + 2);
			if (adate_.compare("2021-08-01") == 0)
			{
				adate_ = "2021-08-02";
			}
			announcementday = adate_;
			//SetDates(announcementday, dateslist, N);

		}

		void SetDates(vector<string> dateslist, int N);

		void SetAbnormalReturns(Index Russel, vector<string> datelist);
		void SetCummulativeAbnormalReturns();
		void GetAbnormalReturns(vector<double>& ret);
		void pullinformation();
		void GetCummulativeAbnormalReturns(vector<double>& ret);

	};


}


