# FRE-GY6883-Project
## Evaluate the impact of quarterly earnings report on stock price movement

• Used liburl to retrieve historical price data from eodhistoricaldata.com. The adjusted close prices for selected Russell 3000 stocks and IWV (Russell 3000 ETF used as market benchmark) is stored into memory.

• Based on the 2nd quarter of 2022 earnings announcement for Russell 3000 stocks
(See the Earnings Announcements sheet), sorted all the surprise% in ascending order, and split all the stocks into 3 groups with relatively equivalent numbers of stocks:
i. Highest surprise group: Beat Estimate Group
ii. Lowest surprise group: Miss Estimate Group
iii. The rest stocks in between: Meet Estimate Group

• Implemented Bootstrapping:
a. Randomly selecting 80 stocks from each group, total 240 stocks.
b. Used libcurl lib to retrieve 2N+1 days of historical prices for Russell 3000 stocks and ETF ticker IWV (used as market benchmark) around the date of earning release. N is integer which must be greater or equal to 60, will be entered by users. Users will be warned if there are not enough historical prices for 2N+1.

• The stocks and their corresponding price information for each group is stored in an STL map, with stock symbol as its keys.

• The expected AAR, AAR STD, and expected CAAR and CAAR STD for 3 groups are calculated and presented in a matrix. 

• Used gnuplot to show the CAAR from all 3 groups in one graph.

• The program has a menu of 5 options:
1. Retrieve Price Data for all the stocks
2. Pull individual stock data
3. Show group data
4. Plot Returns
5. Exit the program

## Flow Chart

![Screen Shot 2022-05-30 at 7 24 24 AM](https://user-images.githubusercontent.com/18114312/170982540-2a4b664f-47d8-423b-94c4-0540319c8025.png)


## Results
![Final_Plot](https://user-images.githubusercontent.com/18114312/170982381-31f34fed-90b4-48a6-9800-7569121ef973.PNG)
