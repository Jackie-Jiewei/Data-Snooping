#pragma once
#include<vector>
#include<string>

class Bootstrap
{
private:
	std::vector<double> IndexReturn;
	std::vector<int> IndexVolume;
public:
	Bootstrap(std::vector<double>&, std::vector<int>&);
	std::tuple<std::vector<double>, std::vector<int>> bootstrap(double q);   //1/q specifies the mean block length
	void stationary_bootstrap(std::vector<double>&, double q, std::string);
	~Bootstrap();
};

//

