#include<iostream>
#include "BootStrap.h"
#include "curl/curl.h"
#include "Returns.h"
#include<string>
#include <cctype>
#include<vector>
#include<algorithm>
#include<set>
#include <chrono>
#include<random>
#include<iterator>
#include <thread>
#include<mutex>
#include<cmath>
#include "Matrix.h"

using namespace std;
bool mycomp(const pair<string, double>& a,const pair<string, double>& b) {

	return a.second > b.second;

}
namespace Team_Project {
	struct MemoryStruct {
		char* memory;
		size_t size;
		size_t total_size;
	};

	void* myrealloc(void* ptr, size_t size)
	{
		if (ptr)
			return realloc(ptr, size);
		else
			return malloc(size);
	}

	static size_t write_data3(void* ptr, size_t size, size_t nmemb, void* data)
	{
		size_t realsize = size * nmemb;
		struct MemoryStruct* mem = (struct MemoryStruct*)data;
		if(((mem->size+realsize) >= mem->total_size))
			mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
		if (mem->memory) {
			memcpy(&(mem->memory[mem->size]), ptr, realsize);
			mem->size += realsize;
			mem->memory[mem->size] = 0;
			if (mem->size > mem->total_size)
				mem->total_size = mem->size;
		}
		return realsize;
	}

	size_t total;
	static size_t write_data2(void* ptr, size_t size, size_t nmemb, void* data)
	{
		size_t realsize = size * nmemb;
		total += realsize;
		struct MemoryStruct* mem = (struct MemoryStruct*)data;
			mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
			//cout << "&&&&&&&&&&&&&SIZE USED: " << mem->size << endl;
		if (mem->memory) {
			memcpy(&(mem->memory[mem->size]), ptr, realsize);
			mem->size += realsize;
			mem->memory[mem->size] = 0;
		}
		return realsize;
	}

	std::map<string, string> months = { { "JAN", "01" },{ "FEB", "02" },{ "MAR", "03" },{ "APR", "04" },
	{ "MAY", "05" },{ "JUN", "06" },{ "JUL", "07" },{ "AUG", "08" },
	{ "SEP", "09" },{ "OCT", "10" },{ "NOV", "11" },{ "DEC", "12" } };

	string GetDate(string Date)
	{

		string eDay = Date.substr(0, 2);
		string eMonth = Date.substr(3, 3);
		for (int i = 0; i < 3; i++)
		{
			eMonth[i] = toupper(eMonth[i]);
		}
		eMonth = months[eMonth];
		string eYear = Date.substr(7, 9);

		string result = "20" + eYear + "-" + eMonth + "-" + eDay;
		return result;
	};

	void BootStrapp::createStockMap(string filename)
	{
		ifstream ifile;
		ifile.open(filename, ios::in);
		if (!ifile.is_open())
			cout << "Error file not opened" << endl;
		//else
			//cout << "open" << endl;
		string line, word;
		vector<string> row;
		ifile.ignore(10000, '\n');
		while (!ifile.eof())
		{
			row.clear();

			getline(ifile, line);
			if (!line.empty()) {

				stringstream s(line);

				while (getline(s, word, ','))
				{
					row.push_back(word);
				}

				string announcement = GetDate(row[1]);
				Stock* st = new Stock(row[0], announcement, biglistdatesIVW, row[3], row[4], row[5], row[6]);
				MyMap.insert(pair<string, Stock*>(row[0], st));
				surprise.push_back(make_pair(row[0], strtod(row[6].c_str(), NULL)));
			}

		}
		ifile.close();
		sort(surprise.begin(), surprise.end(), mycomp);
		Grouping();

	}

	void BootStrapp::Grouping()
	{
		int total = surprise.size();
		int cap_size = floor(total / 3);
		auto itr = surprise.begin();
		int m_size = 1;

		while (m_size <= cap_size && itr != surprise.end())
		{
			string tick = itr->first;
			MyMapBeat.insert(pair<string, Stock*>(tick, MyMap[tick]));
			//cout<<MyMapBeat[tick]->get_ticker_id()<<endl;
			MyMapBeat[tick]->setgroup("Beat");
			//cout << MyMapBeat[tick]->groupName() << endl;
			m_size++; itr++;
		}
		m_size = 1;
		while (m_size <= cap_size && itr != surprise.end())
		{
			string tick = itr->first;
			MyMapSame.insert(pair<string, Stock*>(tick, MyMap[tick]));
			MyMapSame[tick]->setgroup("Meet");
			m_size++; itr++;
		}
		m_size = 1;
		while (itr != surprise.end())
		{
			string tick = itr->first;
			MyMapMiss.insert(pair<string, Stock*>(tick, MyMap[tick]));
			MyMapMiss[tick]->setgroup("Miss");
			m_size++; itr++;
		}

	}
	
	void BootStrapp::PopulateMultiThread() {
		curl_global_init(CURL_GLOBAL_ALL);
		vector<thread> Threads;
		Threads.push_back(thread(&BootStrapp::PopulateStockData, this, MyMapBeat));
		Threads.push_back(thread(&BootStrapp::PopulateStockData, this, MyMapSame));
		Threads.push_back(thread(&BootStrapp::PopulateStockData, this, MyMapMiss));
		for (unsigned int i = 0; i < Threads.size(); i++)
		{
			Threads[i].join();
		}
		cout << "TOTAL MEMORY " << total;
	}
	

	void BootStrapp::PopulateStockData(map<string, Stock*> mp) {

		CURL* handle;
		CURLcode result;
		curl_global_init(CURL_GLOBAL_ALL);

		handle = curl_easy_init();
		if (handle)
		{
			string url_common = "https://eodhistoricaldata.com/api/eod/";

			string api_token = "62670d886346a3.30977754";
			map<string, Stock*>::iterator itr;
			for (itr = mp.begin(); itr != mp.end(); itr++)
			{
				vector<double> Price;
				//cout << itr->first <<endl;
				itr->second->SetDates(biglistdatesIVW, N);
				struct MemoryStruct data;
				data.memory = NULL;
				data.size = 0;
				string symbol = itr->first;
				string start_date = itr->second->GetStartDate();
				string end_date = itr->second->GetEndDate();
				string url_request = url_common + symbol + ".US?" + "from="
					+ start_date + "&to=" + end_date + "&api_token="
					+ api_token + "&period=d";
				curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
				curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
				curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
				curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
				curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
				curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

				result = curl_easy_perform(handle);

				if (result != CURLE_OK) {
					fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
					break;
				}

				curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
				curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

				stringstream sData;
				sData.str(data.memory);
				string sValue, sDate;
				double dValue = 0;
				string line;
				while (getline(sData, line)) {
					size_t found = line.find('-');
					if (found != std::string::npos)
					{
						sDate = line.substr(0, line.find_first_of(','));
						line.erase(line.find_last_of(','));
						sValue = line.substr(line.find_last_of(',') + 1);
						dValue = strtod(sValue.c_str(), NULL);
						Price.push_back(dValue);
					}
				}
				/*
				if (Price.size() == 0) // If data not found delete the stock 
					mp.erase(itr);
					*/
				itr->second->setprices(Price);
				itr->second->CalculateReturns();
				free(data.memory);
				data.size = 0;
			}
		}
		else
		{
			fprintf(stderr, "Curl init failed!\n");
			return;
		}

		curl_easy_cleanup(handle);
		curl_global_cleanup();
		return;
	}


	void BootStrapp::getIVWData()
	{
		vector<double> Price;
		CURL* handle;
		CURLcode result;

		curl_global_init(CURL_GLOBAL_ALL);

		handle = curl_easy_init();

		// if everything's all right with the easy handle...
		if (handle)
		{
			string url_common = "https://eodhistoricaldata.com/api/eod/";
			string api_token = "62670d886346a3.30977754";  // You must replace this API token with yours

			struct MemoryStruct data;
			data.memory = NULL;
			data.size = 0;

			string SYMBOL = "IWV";

			if (SYMBOL.back() == '\r') {
				SYMBOL.pop_back();
			}

			string url_request = url_common + SYMBOL + ".US?" + "from=" + Startdate + "&to=" + Enddate + "&api_token=" + api_token + "&period=d";
			curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());

			//adding a user agent
			curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

			result = curl_easy_perform(handle);

			// check for errors
			if (result != CURLE_OK) {
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				//return -1; Need to STOP EXECUTION HERE;
				return;
			}

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

			stringstream sData;
			sData.str(data.memory);
			string sValue, sDate;
			double dValue = 0;
			string line;
			while (getline(sData, line))
			{
				size_t found = line.find('-');
				if (found != std::string::npos)
				{
					sDate = line.substr(0, line.find_first_of(','));
					line.erase(line.find_last_of(','));
					sValue = line.substr(line.find_last_of(',') + 1);
					dValue = strtod(sValue.c_str(), NULL);
					Price.push_back(dValue);
					biglistdatesIVW.push_back(sDate);
				}
			}
			Russel.setprices(Price);
			Russel.SetDates(biglistdatesIVW);
			free(data.memory);
			data.size = 0;
		}
		else
		{
			fprintf(stderr, "Curl init failed!\n");
			return;
		}
		// cleanup since you've used curl_easy_init
		curl_easy_cleanup(handle);

		// release resources acquired by curl_global_init()
		curl_global_cleanup();

	}
	
	void BootStrapp::CaluclateAbnormalReturns()
	{
		Russel.CalculateReturns();
		for (auto itr = MyMapBeat.begin(); itr != MyMapBeat.end(); itr++)
		{
			itr->second->SetAbnormalReturns(Russel, biglistdatesIVW);
		}
		for (auto itr = MyMapSame.begin(); itr != MyMapSame.end(); itr++)
		{
			itr->second->SetAbnormalReturns(Russel, biglistdatesIVW);
		}
		for (auto itr = MyMapMiss.begin(); itr != MyMapMiss.end(); itr++)
		{
			itr->second->SetAbnormalReturns(Russel, biglistdatesIVW);
		}
	}
	void BootStrapp:: generate_random(int sample_size, vector<int>& sample) {
		
		std::vector<int> numbers;

		for (int i = 0; i < sample_size; i++)   
			numbers.push_back(i);

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));

		for (int i = 0; i < 8; i++)
		{
			sample.push_back(numbers[i]);
		}

	}
	
	void BootStrapp::Calc(returns* r)
	{
		int groupsize = 0;
		map<string, Stock*>::iterator itr;
		map<string, Stock*>::iterator itr1;
		string ss(r->getgroupp());
		string sbeat("Beat");
		string smeet("Meet");
		//cout << ss << endl;
		//ss.push_back('\0');
		int ReturnMatrixIndex;
		if (sbeat.compare(ss) == 0)
		{
			//cout << ss << endl;
			itr = MyMapBeat.begin();
			groupsize = MyMapBeat.size();
			ReturnMatrixIndex = 0;
		}
		else if (smeet.compare(ss) == 0)
		{
			//cout << ss << endl;
			itr = MyMapSame.begin();
			groupsize = MyMapSame.size();
			ReturnMatrixIndex = 1;
		}
		else //if (ss.compare("Miss") == 0)
		{
			//cout << ss << endl;
			itr = MyMapMiss.begin();
			groupsize = MyMapMiss.size();
			ReturnMatrixIndex = 2;
		}
		vector<int> random_sample;
		//cout <<"*******" <<groupsize << "***************** " << r->getgroupp() << endl;
		for (int k = 0; k < 4; k++)
		{
			int n_sample = k;

			generate_random(groupsize, random_sample);
			int sample_size = random_sample.size();

			vector < vector < double >> s_matAbnormal(sample_size, vector<double>((2 * N) + 1, 0));
			vector < vector < double >> s_matCummulativeAbnormal(sample_size, vector<double>((2 * N) + 1, 0));
			s_matAbnormal.resize(sample_size);
			s_matCummulativeAbnormal.resize(sample_size);
			vector<double> Abnormal, CummulativeAbnormal;
			for (int i = 0; i < sample_size; i++)
			{
				itr1 = next(itr, random_sample[i]);
				string temp = itr1->first;
				Stock* t_ptr = MyMap[temp];
				//cout << t_ptr->groupName() << endl;
				t_ptr->GetAbnormalReturns(Abnormal);
				t_ptr->GetCummulativeAbnormalReturns(CummulativeAbnormal);

				for (unsigned int j = 0; j < Abnormal.size(); j++)
				{
					s_matAbnormal[i][j] = Abnormal[j];
					s_matCummulativeAbnormal[i][j] = CummulativeAbnormal[j];
				}
				Abnormal.clear();
				CummulativeAbnormal.clear();
			}
			vector<double> tem;
			vector<double> tem2;

			for (unsigned int i = 0; i < s_matAbnormal[0].size() - 1; i++)
			{
				for (int j = 0; j < sample_size; j++)
				{
					tem.push_back(s_matAbnormal[j][i]);
					tem2.push_back(s_matCummulativeAbnormal[j][i]);
				}
				r->cal_daily_avgs(tem,tem2);
				//r->calc_CAAR();
				tem.clear();
				tem2.clear();
			}
			//r->ARR.clear();
			for (int i = 0; i < sample_size; i++)
			{
				s_matAbnormal[i].clear();
				s_matCummulativeAbnormal[i].clear();
			}
			r->populateAAR(k);
			r->populateCAAR(k);
			s_matAbnormal.clear();
			s_matCummulativeAbnormal.clear();
			//tem.clear();
			//tem2.clear();
			random_sample.clear();

		}
		r->FinalAverages();
		r->StandardDeviations();
		PopulateMatrix(ReturnMatrixIndex, r);

	}
	void BootStrapp::Cal_Groups(returns* r1, returns* r2, returns* r3)
	{
		CaluclateAbnormalReturns();
		ReturnMatrix.clear();
		ReturnMatrix.resize(3);
		for (int i = 0; i < 3; i++)
		{
			ReturnMatrix[i].resize(4, vector<double>(2 * N));
		}
		Calc(r1);
		Calc(r2);
		Calc(r3);
		for (int i = 0; i < 3; i++)
		{
			cout << ReturnMatrix[i] << endl;
		}

	}
	void BootStrapp::PopulateMatrix(int k, returns* r)
	{
		ReturnMatrix[k][0] = r->final_arr;
		ReturnMatrix[k][1] = r->AARStdDevVector;
		ReturnMatrix[k][2]= r->final_caar;
		ReturnMatrix[k][3] = r->CAARStdDevVector;

	 }
	void BootStrapp::plotreturns(int nn, returns* r1, returns* r2, returns* r3)
	{
		FILE* gnuplotPipe, * tempDataFile;
		//char* tempfilename = "MyCAAR";
		const char* tempfilename1 = "Beat";
		const char* tempfilename2 = "Meet";
		const char* tempfilename3 = "Miss";
		int j;
		gnuplotPipe = _popen("F:\\gnuplot\\bin\\gnuplot.exe", "w");

		if (gnuplotPipe)
		{
			fprintf(gnuplotPipe, "plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n", tempfilename1, tempfilename2, tempfilename3);
			fflush(gnuplotPipe);
			tempDataFile = fopen(tempfilename1, "w");
			for (j = -nn; j < nn; j++)
			{
				fprintf(tempDataFile, "%i %lf\n", j, r1->final_caar[j + nn]);
			}
			fclose(tempDataFile);
			tempDataFile;
			tempDataFile = fopen(tempfilename2, "w");

			for (j = -nn; j < nn; j++)
			{
				fprintf(tempDataFile, "%i %lf\n", j, r2->final_caar[j + nn]);
			}
			fclose(tempDataFile);

			tempDataFile = fopen(tempfilename3, "w");

			for (j = -nn; j < nn; j++)
			{
				fprintf(tempDataFile, "%i %lf\n", j, r3->final_caar[j + nn]);
			}
			fclose(tempDataFile);
		}

		else
		{
			printf("gnuplot not found...");
		}
	}

	}







