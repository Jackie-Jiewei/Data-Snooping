#include "UniverseRules.h"
#include "Bootstrap.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>




int main() {

	UniverseRules rules;
	std::string inputfile = "D:\\Rutgers Business School\\First semester\\OOP\\project\\SP500_Daily.csv";
	std::string outputpath = "D:\\Rutgers Business School\\First semester\\OOP\\project\\Project2_bootstrap\\project2_bootsrap\\output";
	rules.readData(inputfile);
//	rules.outputfiles(outputpath);

	std::vector<double> indexreturn = rules.getIndexReturn();
	std::vector<double> riskfree = rules.getRiskFree();
	std::vector<int> indexvolume = rules.getIndexVolume();
	Bootstrap bootstrap(indexreturn, indexvolume);
	for (auto q : { 0.01,0.1,0.5 }) {
		bootstrap.stationary_bootstrap(riskfree, q, outputpath);
	}
	system("PAUSE");
	return 0;
}