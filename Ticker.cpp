#include"Ticker.h"
#include"Matrix.h"
#include<iostream>
#include<vector>
#include<cmath>
#include <assert.h>
#include<numeric>
using namespace std;
namespace Team_Project {

	void Ticker::CalculateReturns()
	{
		Returns.clear();
		for (int i = 1; i < Price.size(); i++) {
			Returns.push_back((Price[i] / Price[i - 1]) - 1);
		}
	}

	void Ticker::setprices(vector<double> prices1)
	{
		Price.clear();
		Price.resize(prices1.size());
		for (int i = 0; i < Price.size(); i++)
		{
			Price[i] = prices1[i];
		}
		//cout << Price.size()<<endl;
	}

	void Ticker::getPrice()
	{
		for (int i = 0; i < Price.size(); i++) {
			cout << Price[i] << endl;
		}
	}

	void Stock::SetAbnormalReturns(Index Russel, vector<string> datelist)
	{
		AbnormalReturns.clear();
		AbnormalReturns.resize(Returns.size());
		auto begindt = find(datelist.begin(), datelist.end(), StartDate);
		auto enddt = find(datelist.begin(), datelist.end(), EndDate);

		int beginidx = distance(datelist.begin(), begindt);
		int endindex = distance(datelist.begin(), enddt);

		vector<double>temp = Russel.GetReturns();
		temp = std::vector<double>(temp.begin() + beginidx, temp.begin() + endindex);
		AbnormalReturns = Returns - temp;

		SetCummulativeAbnormalReturns();
		
	}

	void Stock::SetCummulativeAbnormalReturns()
	{
		CummulativeAbnormalReturns.clear();
		CummulativeAbnormalReturns.resize(AbnormalReturns.size());
		partial_sum(AbnormalReturns.begin(), AbnormalReturns.end(), CummulativeAbnormalReturns.begin());
	}
	void Stock::GetAbnormalReturns(vector<double>& ret) {

		for (auto e : AbnormalReturns)
		{
			ret.push_back(e);
		}
	}

	void Stock::GetCummulativeAbnormalReturns(vector<double>& ret) {

		for (auto e : CummulativeAbnormalReturns)
		{
			ret.push_back(e);
		}
	}
	void Stock::SetDates(vector<string> dateslist, int N)
	{
		auto itr = find(dateslist.begin(), dateslist.end(), announcementday);

		int index = std::distance(dateslist.begin(), itr);
		//cout << index + N - (index - (N + 1))<<endl ;
		StartDate = dateslist[index - N];
		EndDate = dateslist[(index + N)];
	}

	void Stock::pullinformation()
	{
		cout << "Ticker ID " << get_ticker_id() << endl;
		cout << "Daily Prices is " << this->Price << endl;
		cout << "Daily Returns is " << this->Returns << endl;
		cout << "Cummulative Daily Return " << this->CummulativeAbnormalReturns << endl;
		cout << "Belongs to Group " << this->groupName() << endl;
		cout << "Earnings Announcement Date " << this->aday() << endl;
		cout << "Start Date " << this->GetStartDate() << endl;
		cout << "End Date " << this->GetEndDate() << endl;
		cout << "Surprise Ratio " << this->Surprise_Ratio() << endl;
		cout << "Reported Earnings " << this->Reported_Earning() << endl;
		cout << "Estimated Earnings " << this->Estimated_Earning() << endl;
	}

}