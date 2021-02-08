#include "Bootstrap.h"
#include "UniverseRules.h"
#include <random>
#include <ctime>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>



Bootstrap::Bootstrap(std::vector<double>& indexreturn, std::vector<int>& indexvolume) {
	this->IndexReturn = indexreturn;
	this->IndexVolume = indexvolume;
}

std::tuple<std::vector<double>, std::vector<int>> Bootstrap::bootstrap(double q) {
	unsigned int length{ IndexReturn.size() }, theta{ 0 };
	std::vector<double> bootstrap_return;
	std::vector<int> bootstrap_volume;
	std::default_random_engine engine{ static_cast<unsigned int>(time(0)) };
	std::uniform_int_distribution<unsigned int> randomTheta{ 0, length - 1 };
	std::uniform_real_distribution<double> randomU{ 0.0,1.0 };
	double U{ 0.0 };
	
	for (unsigned int t = 0; t < length; t++) {
		if (t == 0) {
			theta = randomTheta(engine);
			bootstrap_return.push_back(IndexReturn[theta]);
			bootstrap_volume.push_back(IndexVolume[theta]);
			continue;
		}
		U = randomU(engine);
		if (U < q) {
			theta = randomTheta(engine);
			bootstrap_return.push_back(IndexReturn[theta]);
			bootstrap_volume.push_back(IndexVolume[theta]);
		}
		else {
			theta += 1;
			if (theta <= length - 1) {
				bootstrap_return.push_back(IndexReturn[theta]);
				bootstrap_volume.push_back(IndexVolume[theta]);
			}
			else {
				theta = 0;
				bootstrap_return.push_back(IndexReturn[theta]);
				bootstrap_volume.push_back(IndexVolume[theta]);
			}
		}
	}
	std::tuple<std::vector<double>, std::vector<int>> tup{ bootstrap_return, bootstrap_volume };
	return tup;
}


void Bootstrap::stationary_bootstrap(std::vector<double>& riskfree, double q, std::string outputpath) {
	std::ofstream isFile;
	std::ostringstream ss;
	ss << "//bootstrap_q" << q << ".csv";
	std::string filename = outputpath + ss.str();
	std::tuple<std::vector<double>, std::vector<std::string>> tup;
	std::vector<double> random_return;
	std::vector<std::string> strategy_name;
	UniverseRules rules;
	rules.setValue(this->IndexReturn, this->IndexVolume, riskfree);
	tup = rules.getRandomReturn();
	random_return = std::get<0>(tup);
	strategy_name = std::get<1>(tup);
	isFile.open(filename);
	isFile << "Name";
	for (unsigned int i = 0; i < strategy_name.size(); i++) {
		isFile << "," << strategy_name[i];
	}
	isFile << std::endl;
	isFile << "Orignial";
	for (unsigned int i = 0; i < random_return.size(); i++) {
		isFile << "," << random_return[i];
	}
	isFile << std::endl;
	for (unsigned int i = 0; i < 2; i++) {
		std::cout << i << std::endl;
		UniverseRules rules;
		std::tuple<std::vector<double>, std::vector<int>> tup2 = bootstrap(q);
		std::vector<double> indexreturn = std::get<0>(tup2);
		std::vector<int> indexvolume = std::get<1>(tup2);
		rules.setValue(indexreturn, indexvolume, riskfree);
		tup = rules.getRandomReturn();
		random_return = std::get<0>(tup);
		std::ostringstream ss;
		ss << "bootstrap scenario" << i + 1;
		isFile << ss.str();
		for (unsigned int j = 0; j < random_return.size(); j++) {
			isFile << "," << random_return[j];
		}
		isFile << std::endl;
	}
	isFile.close();
}


Bootstrap::~Bootstrap()
{
}
