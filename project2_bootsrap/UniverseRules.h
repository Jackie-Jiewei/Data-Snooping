#pragma once
#include <vector>
#include <string>

class UniverseRules
{
private:
	int LENGTH;
	std::vector<double> IndexValue;
	std::vector<double> IndexValue_Exclude_Dividend;
	std::vector<double> RiskFreeRate;
	std::vector<double> IndexReturn;
	std::vector<int> IndexVolume;
	std::vector<std::string> IndexDate;

public:
	UniverseRules();
	void setValue(std::vector<double>&, std::vector<int>&, std::vector<double>&);
	void readData(std::string);
	int getLength() const;
	double innerproduct(std::vector<short> &var1, std::vector<double> var2, std::vector<double> var3)const;
	double vectormean(std::vector<double> var, unsigned int start, unsigned int end) const;
	std::vector<double> getIndexValue() const;
	std::vector<double> getIndexReturn() const;
	std::vector<int> getIndexVolume() const;
	std::vector<std::string> getIndexDate() const;
	std::vector<double> getRiskFree() const;
	std::tuple<std::vector<double>, std::vector<std::string>> getRandomReturn() const;
	void outputfiles(std::string) const;
	//x = change in security price(x * price) required to initiate a position;
	//y = change in security price(y * price) required to liquidate a position;
	//e = used for an alternative definition of extrema where a low(high) can be defined as the most recent closing price 
	//that is less(greater)than the e previous closing prices;
	//c = number of days a position is held, ignoring all other signals during that time;
	std::vector<short> FilterRulesPosition(double x, double y, unsigned int e, unsigned int c) const; //return 1(long), 0(neutral), -1(short)

	//e = used for an alternative definition of extrema where a low(high) can be defined as the most recent closing price 
	//that is less(greater)than the e previous closing prices;
	//c = number of days a position is held, ignoring all other signals during that time;
	//n = number of days in the support and resistance range;
	//b = fixed band multiplicative value;
	//d = number of days for the time delay filter;
	std::vector<short> SandRRulesPositition(unsigned int n, unsigned int e, double b, unsigned int c, unsigned int d) const;

	//n = number of days for the channel;
	//x = difference between the high price and the low price (x * high price) required to form a channel;
	//b = fixed band multiplicative value;
	//c = number of days a position is held, ignoring all other signals during that time;
	std::vector<short> ChannelRulesPosition(unsigned int n, double x, double b, unsigned int c) const;

	//n = number of days in a fast moving average;
	//m = number of days in a slow moving average;
	//b = fixed band multiplicative value;
	//d = number of days for the time delay filter;
	//c = number of days
	std::vector<short> MovingAverageRulesPosition(unsigned int n, unsigned int m, double b, unsigned int c, unsigned int d) const;

	//n = number of days in a fast moving average;
	//m = number of days in a slow moving average;
	//b = fixed band multiplicative value;
	//d = number of days for the time delay filter;
	//c = number of days
	std::vector<double> getOBV() const;
	std::vector<short> OBVRulesPosition(unsigned int n, unsigned int m, double b, unsigned int c, unsigned int d) const;


	~UniverseRules();
};

