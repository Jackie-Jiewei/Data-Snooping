#include "UniverseRules.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <numeric>
#include <tuple>
#include <math.h>

UniverseRules::UniverseRules() {
};

void UniverseRules::setValue(std::vector<double>& indexreturn, std::vector<int>& indexvolume, std::vector<double>& riskfree) {
	this->IndexReturn = indexreturn;
	this->IndexVolume = indexvolume;
	this->RiskFreeRate = riskfree;
	for (unsigned int i = 0; i < IndexReturn.size(); i++) {
		if (i == 0) IndexValue.push_back(1);
		else IndexValue.push_back(IndexValue[i - 1] + IndexReturn[i]);
	}
	LENGTH = IndexReturn.size();
}
double UniverseRules::innerproduct(std::vector<short> &var1, std::vector<double> var2, std::vector<double> var3)const {
	if (var1.size() != var2.size()) {
		std::cout << "Not Applicable";
		return 0;
	}
	else {
		double sum = 0;
		for (unsigned int i = 0; i < var1.size(); i++) {
			sum += var1[i] * (var2[i] - var3[i]);
		}
		return sum;
	}
}
double UniverseRules::vectormean(std::vector<double> var, unsigned int start, unsigned int end) const {
	if (var.size() < end) { std::cout << "vector out of range"; }
	double sum = 0.0;
	for (unsigned int i = start; i < end; i++) {
		sum += var[i];
	}
	return sum / (end - start);
}
int UniverseRules::getLength() const {
	return LENGTH;
}
std::vector<double> UniverseRules::getIndexValue() const{
	return IndexValue;
}
std::vector<double> UniverseRules::getIndexReturn() const{
	return IndexReturn;
}
std::vector<double> UniverseRules::getRiskFree() const {
	return RiskFreeRate;
}
std::vector<int> UniverseRules::getIndexVolume() const{
	return IndexVolume;
}
std::vector<std::string> UniverseRules::getIndexDate() const{
	return IndexDate;
}

std::tuple<std::vector<double>, std::vector<std::string>> UniverseRules::getRandomReturn() const {
	std::vector<short> position;
	std::vector<double> acc_return(IndexValue.size());
	std::vector<double> random_return;
	std::vector<std::string> strategy_name;
	double daily_return;
	for (auto x : { 0.005, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05, 0.06, 0.07,
		0.08, 0.09, 0.1, 0.12, 0.14, 0.16, 0.18, 0.2, 0.25, 0.3, 0.4, 0.5 }) {
		for (auto y : { 0.0, 0.005, 0.01, 0.015, 0.02, 0.025, 0.03, 0.04, 0.05, 0.075, 0.1, 0.15, 0.2 }) {
			for (auto e : { 0, 1, 2, 3, 4, 5, 10, 15, 20 }) {
				for (auto c : { 0, 5, 10, 25, 50 }) {
					if ((y != 0 && y < x && e == 0 && c == 0) || (y == 0 && e != 0 && c == 0)\
						|| (y == 0 && e == 0 && c != 0) || (y == 0 && e == 0 && c == 0)) {
						position = FilterRulesPosition(x, y, e, c);
						daily_return = innerproduct(position, IndexReturn, RiskFreeRate) / LENGTH;
						random_return.push_back(daily_return);
						std::ostringstream ss;
						ss << "Filter" << "x" << x << "y" << y << "e" << e << "c" << c;
						strategy_name.push_back(ss.str());
					}
				}
			}
		}
	}
	for (auto n : { 0, 5, 10, 15, 20, 25, 50, 100, 150, 200, 250 }) {
		for (auto e : { 0, 2, 3, 4, 5, 10, 20, 25, 50, 100, 200 }) {
			for (auto b : { 0.0, 0.001, 0.005, 0.01, 0.015, 0.02, 0.03, 0.04, 0.05 }) {
				for (auto c : { 0, 5, 10, 25, 50 }) {
					for (auto d : { 0, 2, 3, 4, 5 }) {
						if ((n == 0 && e != 0 && (b == 0 || d == 0)) || (e == 0 && n != 0 && (b == 0 || d == 0))) {
							position = SandRRulesPositition(n, e, b, c, d);
							daily_return = innerproduct(position, IndexReturn, RiskFreeRate) / LENGTH;
							random_return.push_back(daily_return);
							std::ostringstream ss;
							ss << "SandR" << "n" << n << "e" << e << "b" << b << "c" << c << "d" << d;
							strategy_name.push_back(ss.str());
						}
					}
				}
			}
		}
	}
	//for (auto n : { 5, 10, 15, 20, 25, 50, 100, 150, 200, 250 }) {
	//	for (auto x : { 0.005, 0.01, 0.02, 0.03, 0.05, 0.075, 0.10, 0.15 }) {
	//		for (auto b : { 0.0, 0.001, 0.005, 0.01, 0.015, 0.02, 0.03, 0.04, 0.05 }) {
	//			for (auto c : { 5, 10, 25, 50 }) {
	//				if (b < x) {
	//					position = ChannelRulesPosition(n, x, b, c);
	//					daily_return = innerproduct(position, IndexReturn, RiskFreeRate) / LENGTH;
	//					random_return.push_back(daily_return);
//				}
//			}
//		}
//	}
//}

	for (auto n : { 1, 2, 5, 10, 15, 20, 25, 30, 40, 50, 75, 100, 125, 150, 200, 250 }) {
		for (auto m : { 1, 2, 5, 10, 15, 20, 25, 30, 40, 50, 75, 100, 125, 150, 200, 250 }) {
			for (auto b : { 0.0, 0.001, 0.005, 0.01, 0.015, 0.02, 0.03, 0.04, 0.05 }) {
				for (auto c : { 0, 5, 10, 25, 50 }) {
					for (auto d : { 0, 2, 3, 4, 5 }) {
						if (n < m && ((b == 0 && c == 0 && d == 0) || (b != 0 && c == 0 && d == 0) || \
							(b == 0 && c != 0 && d == 0) || (b == 0 && c == 0 && d != 0))) {
							position = MovingAverageRulesPosition(n, m, b, c, d);
							daily_return = innerproduct(position, IndexReturn, RiskFreeRate) / LENGTH;
							random_return.push_back(daily_return);
							std::ostringstream ss;
							ss << "MA" << "n" << n << "m" << m << "b" << b << "c" << c << "d" << d;
							strategy_name.push_back(ss.str());
						}
					}
				}
			}
		}
	}

	for (auto n : { 1, 2, 5, 10, 15, 20, 25, 30, 40, 50, 75, 100, 125, 150, 200, 250 }) {
		for (auto m : { 1, 2, 5, 10, 15, 20, 25, 30, 40, 50, 75, 100, 125, 150, 200, 250 }) {
			for (auto b : { 0.0, 0.001, 0.005, 0.01, 0.015, 0.02, 0.03, 0.04, 0.05 }) {
				for (auto c : { 0, 5, 10, 25, 50 }) {
					for (auto d : { 0, 2, 3, 4, 5 }) {
						if (n < m && ((b == 0 && c == 0 && d == 0) || (b != 0 && c == 0 && d == 0) || \
							(b == 0 && c != 0 && d == 0) || (b == 0 && c == 0 && d != 0))) {
							position = OBVRulesPosition(n, m, b, c, d);
							daily_return = innerproduct(position, IndexReturn, RiskFreeRate) / LENGTH;
							random_return.push_back(daily_return);
							std::ostringstream ss;
							ss << "OBV" << "n" << n << "m" << m << "b" << b << "c" << c << "d" << d;
							strategy_name.push_back(ss.str());
						}
					}
				}
			}
		}
	}
	std::tuple<std::vector<double>, std::vector<std::string>> tup{ random_return, strategy_name };
	return tup;
}

void UniverseRules::outputfiles(std::string outputpath) const {
	std::vector<short> position;
	std::vector<double> acc_return(IndexValue.size());
	std::vector<double> random_return;
	double daily_return;
	std::ofstream isFile;
	for (auto x : { 0.005, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05, 0.06, 0.07,
		0.08, 0.09, 0.1, 0.12, 0.14, 0.16, 0.18, 0.2, 0.25, 0.3, 0.4, 0.5 }) {
		for (auto y : { 0.0, 0.005, 0.01, 0.015, 0.02, 0.025, 0.03, 0.04, 0.05, 0.075, 0.1, 0.15, 0.2 }) {
			for (auto e : { 0, 1, 2, 3, 4, 5, 10, 15, 20 }) {
				for (auto c : { 0, 5, 10, 25, 50 }) {
					if ((y != 0 && y < x && e == 0 && c == 0) || (y == 0 && e != 0 && c == 0)\
						|| (y == 0 && e == 0 && c != 0) || (y == 0 && e == 0 && c == 0)) {
						position = FilterRulesPosition(x, y, e, c);
						daily_return = innerproduct(position, IndexReturn, RiskFreeRate) / LENGTH;
						random_return.push_back(daily_return);
						if (daily_return > 0.0006) {
							for (unsigned int i = 0; i < position.size(); i++) {
								if (i == 0) acc_return[i] = 1;
								else {
									acc_return[i] = acc_return[i - 1] + position[i] * (IndexReturn[i] - RiskFreeRate[i]);
								}
							}
							std::ostringstream ss;
							std::string file_name;
							ss << outputpath << "\\FilterRules\\" \
								<< "x" << x << "y" << y << "e" << e << "c" << c << "output.csv";
							file_name = ss.str();
							isFile.open(file_name);
							isFile << "Date,Index_ID,Index_ED,Position,Acc_Return,Risk_Free" << std::endl;
							for (unsigned int i = 0; i < position.size(); i++) isFile << IndexDate[i] << "," << IndexValue[i] \
								<< "," << IndexValue_Exclude_Dividend[i] << "," << position[i] << "," << acc_return[i] \
								<< "," << RiskFreeRate[i] << std::endl;
							isFile.close();
						}
					}
				}
			}
		}
	}
	for (auto n : { 0, 5, 10, 15, 20, 25, 50, 100, 150, 200, 250 }) {
		for (auto e : { 0, 2, 3, 4, 5, 10, 20, 25, 50, 100, 200 }) {
			for (auto b : { 0.0, 0.001, 0.005, 0.01, 0.015, 0.02, 0.03, 0.04, 0.05 }) {
				for (auto c : { 0, 5, 10, 25, 50 }) {
					for (auto d : { 0, 2, 3, 4, 5 }) {
						if ((n == 0 && e != 0 && (b == 0 || d == 0)) || (e == 0 && n != 0 && (b == 0 || d == 0))) {
							position = SandRRulesPositition(n, e, b, c, d);
							daily_return = innerproduct(position, IndexReturn, RiskFreeRate) / LENGTH;
							random_return.push_back(daily_return);
							if (daily_return > 0.0006) {
								for (unsigned int i = 0; i < position.size(); i++) {
									if (i == 0) acc_return[i] = 1;
									else {
										acc_return[i] = acc_return[i - 1] + position[i] * (IndexReturn[i] - RiskFreeRate[i]);
									}
								}
								std::ostringstream ss;
								std::string file_name;
								ss << outputpath << "\\SandR\\" \
									<< "n" << n << "e" << e << "b" << b << "c" << c << "d" << d << "output.csv";
								file_name = ss.str();
								isFile.open(file_name);
								isFile << "Date,Index_ID,Index_ED,Position,Acc_Return,Risk_Free" << std::endl;
								for (unsigned int i = 0; i < position.size(); i++) isFile << IndexDate[i] << "," << IndexValue[i] \
									<< "," << IndexValue_Exclude_Dividend[i] << "," << position[i] << "," << acc_return[i] \
									<< "," << RiskFreeRate[i] << std::endl;
								isFile.close();

							}
						}
					}
				}
			}
		}
	}
	//for (auto n : { 5, 10, 15, 20, 25, 50, 100, 150, 200, 250 }) {
	//	for (auto x : { 0.005, 0.01, 0.02, 0.03, 0.05, 0.075, 0.10, 0.15 }) {
	//		for (auto b : { 0.0, 0.001, 0.005, 0.01, 0.015, 0.02, 0.03, 0.04, 0.05 }) {
	//			for (auto c : { 5, 10, 25, 50 }) {
	//				if (b < x) {
	//					position = ChannelRulesPosition(n, x, b, c);
	//					daily_return = innerproduct(position, IndexReturn, RiskFreeRate) / LENGTH;
	//					random_return.push_back(daily_return);
	//					if (daily_return > 0.0004) {
	//						for (unsigned int i = 0; i < position.size(); i++) {
	//							if (i == 0) acc_return[i] = 1;
	//							else {
	//								acc_return[i] = acc_return[i - 1] + position[i] * (IndexReturn[i] - RiskFreeRate[i]);
	//							}
	//						}
	//						std::ostringstream ss;
	//						std::string file_name;
	//						ss << outputpath << "\\ChannelRules\\" \
	//							<< "n" << n << "x" << x << "b" << b << "c" << c << "output.csv";
	//						file_name = ss.str();
	//						isFile.open(file_name);
	//						isFile << "Date,Index_ID,Index_ED,Position,Acc_Return,Risk_Free" << std::endl;
	//						for (unsigned int i = 0; i < position.size(); i++) isFile << IndexDate[i] << "," << IndexValue[i] \
	//								<< "," << IndexValue_Exclude_Dividend[i] << "," << position[i] << "," << acc_return[i] \
	//							<< "," << RiskFreeRate[i] << std::endl;
	//						isFile.close();
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	for (auto n : { 1, 2, 5, 10, 15, 20, 25, 30, 40, 50, 75, 100, 125, 150, 200, 250 }) {
		for (auto m : { 1, 2, 5, 10, 15, 20, 25, 30, 40, 50, 75, 100, 125, 150, 200, 250 }) {
			for (auto b : { 0.0, 0.001, 0.005, 0.01, 0.015, 0.02, 0.03, 0.04, 0.05 }) {
				for (auto c : { 0, 5, 10, 25, 50 }) {
					for (auto d : { 0, 2, 3, 4, 5 }) {
						if (n < m && ((b == 0 && c == 0 && d == 0) || (b != 0 && c == 0 && d == 0) || \
							(b == 0 && c != 0 && d == 0) || (b == 0 && c == 0 && d != 0))) {
							position = MovingAverageRulesPosition(n, m, b, c, d);
							daily_return = innerproduct(position, IndexReturn, RiskFreeRate) / LENGTH;
							random_return.push_back(daily_return);
							if (daily_return > 0.0006) {
								for (unsigned int i = 0; i < position.size(); i++) {
									if (i == 0) acc_return[i] = 1;
									else {
										acc_return[i] = acc_return[i - 1] + position[i] * (IndexReturn[i] - RiskFreeRate[i]);
									}
								}
								std::ostringstream ss;
								std::string file_name;
								ss << outputpath << "\\MovingAverage\\" \
									<< "n" << n << "m" << m << "b" << b << "c" << c << "d" << d << "output.csv";
								file_name = ss.str();
								isFile.open(file_name);
								isFile << "Date,Index_ID,Index_ED,Position,Acc_Return,Risk_Free" << std::endl;
								for (unsigned int i = 0; i < position.size(); i++) isFile << IndexDate[i] << "," << IndexValue[i] \
									<< "," << IndexValue_Exclude_Dividend[i] << "," << position[i] << "," << acc_return[i] \
									<< "," << RiskFreeRate[i] << std::endl;
								isFile.close();

							}
						}
					}
				}
			}
		}
	}

	for (auto n : { 1, 2, 5, 10, 15, 20, 25, 30, 40, 50, 75, 100, 125, 150, 200, 250 }) {
		for (auto m : { 1, 2, 5, 10, 15, 20, 25, 30, 40, 50, 75, 100, 125, 150, 200, 250 }) {
			for (auto b : { 0.0, 0.001, 0.005, 0.01, 0.015, 0.02, 0.03, 0.04, 0.05 }) {
				for (auto c : { 0, 5, 10, 25, 50 }) {
					for (auto d : { 0, 2, 3, 4, 5 }) {
						if (n < m && ((b == 0 && c == 0 && d == 0) || (b != 0 && c == 0 && d == 0) || \
							(b == 0 && c != 0 && d == 0) || (b == 0 && c == 0 && d != 0))) {
							position = OBVRulesPosition(n, m, b, c, d);
							daily_return = innerproduct(position, IndexReturn, RiskFreeRate) / LENGTH;
							random_return.push_back(daily_return);
							if (daily_return > 0.0006) {
								for (unsigned int i = 0; i < position.size(); i++) {
									if (i == 0) acc_return[i] = 1;
									else {
										acc_return[i] = acc_return[i - 1] + position[i] * (IndexReturn[i] - RiskFreeRate[i]);
									}
								}
								std::ostringstream ss;
								std::string file_name;
								ss << outputpath << "\\OBV\\" \
									<< "n" << n << "m" << m << "b" << b << "c" << c << "d" << d << "output.csv";
								file_name = ss.str();
								isFile.open(file_name);
								isFile << "Date,Index_ID,Index_ED,Position,Acc_Return,Risk_Free" << std::endl;
								for (unsigned int i = 0; i < position.size(); i++) isFile << IndexDate[i] << "," << IndexValue[i] \
									<< "," << IndexValue_Exclude_Dividend[i] << "," << position[i] << "," << acc_return[i] \
									<< "," << RiskFreeRate[i] << std::endl;
								isFile.close();

							}
						}
					}
				}
			}
		}
	}
	std::string filename = "\\randomreturn.csv";
	isFile.open(outputpath + filename);
	for (unsigned int i = 0; i < random_return.size(); i++) isFile << random_return[i] << std::endl;
	isFile.close();
	random_return.clear();
}
void UniverseRules::readData(std::string inputfile) {
	std::ifstream inFile(inputfile, std::ios::in);
	std::string lineStr;
	int line = 0;
	while (getline(inFile, lineStr) && line < 20000) {
		if (line < 0) {
			line++;
			continue;
		}
		std::stringstream ss(lineStr);
		std::string str;
		int index = 0;
		while (getline(ss, str, ',')) {
			if (index == 0) IndexDate.push_back(str);
			if (index == 3) IndexValue.push_back(std::stod(str));
			if (index == 4) IndexValue_Exclude_Dividend.push_back(std::stod(str));
			if (index == 5) RiskFreeRate.push_back(std::stod(str));
			if (index == 6) IndexVolume.push_back(std::stoi(str));
			index = (index + 1) % 7;
		}
		if (0 == line) IndexReturn.push_back(0);
		else IndexReturn.push_back(IndexValue[line - 0] - IndexValue[line - 1]);
		line++;
	}
	LENGTH = line - 0;
	for (unsigned int i = 0; i < IndexReturn.size(); i++) {
		if (i != 0) {
			IndexValue[i] = IndexValue[i] - IndexValue[0] + 1;
			IndexValue_Exclude_Dividend[i] = IndexValue_Exclude_Dividend[i] - IndexValue_Exclude_Dividend[0] + 1;
		}
	}
	IndexValue[0] = 1;
	IndexValue_Exclude_Dividend[0] = 1;
}

std::vector<short> UniverseRules::FilterRulesPosition(double x, double y, unsigned int e, unsigned int c) const {
	double localmax = IndexValue[0];
	double localmin = IndexValue[0];
	std::vector<short> position(IndexValue.size());
	position[0] = 0;
	if (y != 0 && e == 0 && c == 0) {
		for (unsigned int i = 1; i < position.size(); i++) {
			localmax = std::max(localmax, IndexValue[i - 1]);
			localmin = std::min(localmin, IndexValue[i - 1]);
			if (position[i - 1] == 0 && IndexValue[i - 1] / localmin - 1 > x) {
				position[i] = 1;
				localmax = IndexValue[i - 1];
			}
			else if (position[i - 1] == 0 && 1 - IndexValue[i - 1] / localmax > x) {
				position[i] = -1;
				localmin = IndexValue[i - 1];
			}
			else if (position[i - 1] == 1 && 1 - IndexValue[i - 1] / localmax > y) {
				position[i] = 0;
				localmin = IndexValue[i - 1];
			}
			else if (position[i - 1] == -1 && IndexValue[i - 1] / localmin - 1 > y) {
				position[i] = 0;
				localmax = IndexValue[i - 1];
			}
			else position[i] = position[i - 1];
		}
	}
	else if (e != 0 && y == 0 && c == 0) {
		//find e-max(min) vector
		std::vector<double> max_vector(position.size());
		std::vector<double> min_vector(position.size());
		bool max_flag = false;
		bool min_flag = false;
		for (unsigned int i = e; i < IndexValue.size(); i++) {
			if (i == e) {
				localmax = *std::max_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
				localmin = *std::min_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
			}
			else {
				if (IndexValue[i] >= localmax) {
					localmax = IndexValue[i];
					max_vector[i] = IndexValue[i];
					min_vector[i] = min_vector[i - 1];
				}
				else if (IndexValue[i] <= localmin) {
					localmin = IndexValue[i];
					min_vector[i] = IndexValue[i];
					max_vector[i] = max_vector[i - 1];
				}
				else {
					max_vector[i] = max_vector[i - 1];
					min_vector[i] = min_vector[i - 1];
					if (max_flag) {
						localmax = *std::max_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
						max_flag = false;
					}
					if (min_flag) {
						localmin = *std::min_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
						min_flag = false;
					}
				}
			}
			if (localmax == IndexValue[i - e]) max_flag = true;
			if (localmin == IndexValue[i - e]) min_flag = true;
		}
		for (unsigned int i = 2; i < position.size(); i++) {
			if (max_vector[i - 2] != 0 && IndexValue[i - 1] > (1 + x) * min_vector[i - 2]) position[i] = 1;
			else if (min_vector[i - 2] != 0 && IndexValue[i - 1] < (1 - x) * max_vector[i - 2]) position[i] = -1;
			else position[i] = position[i - 1];
		}
	}
	else if (c != 0 && y == 0 && e == 0) {
		int days = 100;
		bool Flag = true; //whether signal is allowed
		for (unsigned int i = 1; i < position.size(); i++) {
			localmax = std::max(localmax, IndexValue[i - 1]);
			localmin = std::min(localmin, IndexValue[i - 1]);
			if (c == days && Flag == false) {
				if (position[i - 1] == 1) localmin = IndexValue[i - 1];
				else localmax = IndexValue[i - 1];
				Flag = true;
				position[i] = 0;
				days = 100;
				continue;
			}
			if (Flag == false) position[i] = position[i - 1];
			if (IndexValue[i - 1] / localmin - 1 > x) {
				if (Flag) {
					position[i] = 1;
					Flag = false;
					days = 0;
				}
				localmax = IndexValue[i - 1];
			}
			else if (1 - IndexValue[i - 1] / localmax > x) {
				if (Flag) {
					position[i] = -1;
					Flag = false;
					days = 0;
				}
				localmin = IndexValue[i - 1];
			}
			else position[i] = position[i - 1];
			days++;
		}
	}
	else if (e == 0 && c == 0 && y == 0) {
		for (unsigned int i = 1; i < position.size(); i++) {
			localmax = std::max(localmax, IndexValue[i - 1]);
			localmin = std::min(localmin, IndexValue[i - 1]);
			if ((position[i - 1] == 0 || position[i - 1] == -1) && IndexValue[i - 1] / localmin - 1 > x) {
				position[i] = 1;
				localmax = IndexValue[i - 1];
			}
			else if ((position[i - 1] == 0 || position[i - 1] == 1) && 1 - IndexValue[i - 1] / localmax > x) {
				position[i] = -1;
				localmin = IndexValue[i - 1];
			}
			else position[i] = position[i - 1];
		}
	}
	else {
		std::cout << "There can be at most one non-zero in y,e,and c" << std::endl;
	}

	return position;
}
std::vector<short> UniverseRules::SandRRulesPositition(unsigned int n, unsigned int e, double b, unsigned int c, unsigned int d) const {
	double localmax = IndexValue[0];
	double localmin = IndexValue[0];
	std::vector<short> position(IndexValue.size());
	position[0] = 0;
	if (b != 0 && d != 0) {
		std::cout << "b, d can't be all non zero" << std::endl;
		return position;
	}
	if (c == 0 && e == 0 && n != 0) {
		bool max_flag = false;
		bool min_flag = false;
		if (b == 0 && d == 0) {
			for (unsigned int i = n + 1; i < position.size(); i++) {
				if (i == n + 1) {
					localmax = *std::max_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
					localmin = *std::min_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
				}
				if (IndexValue[i - 1] > localmax) position[i] = 1;
				else if (IndexValue[i - 1] < localmin) position[i] = -1;
				else position[i] = position[i - 1];
				if (i != n + 1) {
					if (IndexValue[i - 1] >= localmax) localmax = IndexValue[i - 1];
					else if (IndexValue[i - 1] <= localmin) localmin = IndexValue[i - 1];
					else {
						if (max_flag) {
							localmax = *std::max_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
							max_flag = false;
						}
						if (min_flag) {
							localmin = *std::min_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
							min_flag = false;
						}
					}
				}
				if (localmax == IndexValue[i - n - 1]) max_flag = true;
				if (localmin == IndexValue[i - n - 1]) min_flag = true;

			}
		}
		else if (b == 0 && d != 0) {
			for (unsigned int i = n + d; i < position.size(); i++) {
				if (i == n + d) {
					localmax = *std::max_element(IndexValue.begin() + i - d - n, IndexValue.begin() + i - d);
					localmin = *std::min_element(IndexValue.begin() + i - d - n, IndexValue.begin() + i - d);
				}
				double min_signal = *std::min_element(IndexValue.begin() + i - d, IndexValue.begin() + i);
				double max_signal = *std::max_element(IndexValue.begin() + i - d, IndexValue.begin() + i);
				if (min_signal > localmax) position[i] = 1;
				else if (max_signal < localmin) position[i] = -1;
				else position[i] = position[i - 1];
				if (i != n + d) {
					if (IndexValue[i - d] >= localmax) localmax = IndexValue[i - d];
					else if (IndexValue[i - d] <= localmin) localmin = IndexValue[i - d];
					else {
						if (max_flag) {
							localmax = *std::max_element(IndexValue.begin() + i - d - n, IndexValue.begin() + i - d);
							max_flag = false;
						}
						if (min_flag) {
							localmin = *std::min_element(IndexValue.begin() + i - d - n, IndexValue.begin() + i - d);
							min_flag = false;
						}
					}
				}
				if (localmax == IndexValue[i - n - d]) max_flag = true;
				if (localmin == IndexValue[i - n - d]) min_flag = true;
			}
		}
		else if (b != 0 && d == 0) {
			for (unsigned int i = n + 1; i < position.size(); i++) {
				if (i == n + 1) {
					localmax = *std::max_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
					localmin = *std::min_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
				}
				if (IndexValue[i - 1] >(1 + b) * localmax) position[i] = 1;
				else if (IndexValue[i - 1] < (1 - b) * localmin) position[i] = -1;
				else position[i] = position[i - 1];
				if (i != n + 1) {
					if (IndexValue[i - 1] >= localmax) localmax = IndexValue[i - 1];
					else if (IndexValue[i - 1] <= localmin) localmin = IndexValue[i - 1];
					else {
						if (max_flag) {
							localmax = *std::max_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
							max_flag = false;
						}
						if (min_flag) {
							localmin = *std::min_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
							min_flag = false;
						}
					}
				}
				if (localmax == IndexValue[i - n - 1]) max_flag = true;
				if (localmin == IndexValue[i - n - 1]) min_flag = true;
			}
		}
	}
	else if (c == 0 && e != 0 && n == 0) {
		//find e-max(min) vector
		std::vector<double> max_vector(position.size());
		std::vector<double> min_vector(position.size());
		bool max_flag = false;
		bool min_flag = false;
		for (unsigned int i = e; i < IndexValue.size(); i++) {
			if (i == e) {
				localmax = *std::max_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
				localmin = *std::min_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
			}
			else {
				if (IndexValue[i] >= localmax) {
					localmax = IndexValue[i];
					max_vector[i] = IndexValue[i];
					min_vector[i] = min_vector[i - 1];
				}
				else if (IndexValue[i] <= localmin) {
					localmin = IndexValue[i];
					min_vector[i] = IndexValue[i];
					max_vector[i] = max_vector[i - 1];
				}
				else {
					max_vector[i] = max_vector[i - 1];
					min_vector[i] = min_vector[i - 1];
					if (max_flag) {
						localmax = *std::max_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
						max_flag = false;
					}
					if (min_flag) {
						localmin = *std::min_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
						min_flag = false;
					}
				}
			}
			if (localmax == IndexValue[i - e]) max_flag = true;
			if (localmin == IndexValue[i - e]) min_flag = true;
		}
		if (b == 0 && d == 0) {
			for (unsigned int i = 2; i < position.size(); i++) {
				if (max_vector[i - 2] != 0 && IndexValue[i - 1] > max_vector[i - 2]) position[i] = 1;
				else if (min_vector[i - 2] != 0 && IndexValue[i - 1] < min_vector[i - 2]) position[i] = -1;
				else position[i] = position[i - 1];
			}
		}
		else if (b != 0 && d == 0) {
			for (unsigned int i = 2; i < position.size(); i++) {
				if (max_vector[i - 2] != 0 && IndexValue[i - 1] > (1 + b) * max_vector[i - 2]) position[i] = 1;
				else if (min_vector[i - 2] != 0 && IndexValue[i - 1] < (1 - b ) * min_vector[i - 2]) position[i] = -1;
				else position[i] = position[i - 1];
			}
		}
		else if (b == 0 && d != 0) {
			for (unsigned int i = d + 1; i < position.size(); i++) {
				double min_signal = *std::min_element(IndexValue.begin() + i - d, IndexValue.begin() + i);
				double max_signal = *std::max_element(IndexValue.begin() + i - d, IndexValue.begin() + i);
				if (max_vector[i - d - 1] != 0 && min_signal > max_vector[i - d - 1]) position[i] = 1;
				else if (min_vector[i - d - 1] != 0 && max_signal < min_vector[i - d - 1]) position[i] = -1;
				else position[i] = position[i - 1];
			}
		}
	}
	else if (c != 0 && e == 0 && n != 0) {
		bool Flag = true;
		bool max_flag = false;
		bool min_flag = false;
		int days = 100;
		if (b == 0 && d == 0) {
			for (unsigned int i = n + 1; i < position.size(); i++) {

				if (Flag == false && days == c) {
					Flag = true;
					position[i] = 0;
					days = 100;
					continue;
				}
				if (i == n + 1) {
					localmax = *std::max_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
					localmin = *std::min_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
				}
				if (IndexValue[i - 1] > localmax && Flag == true) {
					position[i] = 1;
					Flag = false;
					days = 0;
				}
				else if (IndexValue[i - 1] < localmin && Flag == true) {
					position[i] = -1;
					Flag = false;
					days = 0;
				}
				else position[i] = position[i - 1];
				if (i != n + 1) {
					if (IndexValue[i - 1] >= localmax) localmax = IndexValue[i - 1];
					else if (IndexValue[i - 1] <= localmin) localmin = IndexValue[i - 1];
					else {
						if (max_flag) {
							localmax = *std::max_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
							max_flag = false;
						}
						if (min_flag) {
							localmin = *std::min_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
							min_flag = false;
						}
					}
				}
				if (localmax == IndexValue[i - n - 1]) max_flag = true;
				if (localmin == IndexValue[i - n - 1]) min_flag = true;
				days++;
			}
		}
		else if (b == 0 && d != 0) {
			for (unsigned int i = n + d; i < position.size(); i++) {

				if (Flag == false && days == c) {
					Flag = true;
					position[i] = 0;
					days = 100;
					continue;
				}
				if (i == n + d) {
					localmax = *std::max_element(IndexValue.begin() + i - d - n, IndexValue.begin() + i - d);
					localmin = *std::min_element(IndexValue.begin() + i - d - n, IndexValue.begin() + i - d);
				}
				double min_signal = *std::min_element(IndexValue.begin() + i - d, IndexValue.begin() + i);
				double max_signal = *std::max_element(IndexValue.begin() + i - d, IndexValue.begin() + i);
				if (min_signal > localmax && Flag == true) {
					position[i] = 1;
					Flag = false;
					days = 0;
				}
				else if (max_signal < localmin && Flag == true) {
					position[i] = -1;
					Flag = false;
					days = 0;
				}
				else position[i] = position[i - 1];

				if (n != n + d) {
					if (IndexValue[i - d] >= localmax) localmax = IndexValue[i - d];
					else if (IndexValue[i - d] <= localmin) localmin = IndexValue[i - d];
					else {
						if (max_flag) {
							localmax = *std::max_element(IndexValue.begin() + i - d - n, IndexValue.begin() + i - d);
							max_flag = false;
						}
						if (min_flag) {
							localmin = *std::min_element(IndexValue.begin() + i - d - n, IndexValue.begin() + i - d);
							min_flag = false;
						}
					}
				}
				if (localmax == IndexValue[i - n - d]) max_flag = true;
				if (localmin == IndexValue[i - n - d]) min_flag = true;
				days++;
			}
		}
		else if (b != 0 && d == 0) {
			for (unsigned int i = n + 1; i < position.size(); i++) {
				if (Flag == false && days == c) {
					Flag = true;
					position[i] = 0;
					days = 100;
					continue;
				}
				if (i == n + 1) {
					localmax = *std::max_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
					localmin = *std::min_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
				}
				if (IndexValue[i - 1] > (1 + b) * localmax && Flag == true) {
					position[i] = 1;
					Flag = false;
					days = 0;
				}
				else if (IndexValue[i - 1] < (1 - b) * localmin && Flag == true) {
					position[i] = -1;
					Flag = false;
					days = 0;
				}
				else position[i] = position[i - 1];
				if (i != n + 1) {
					if (IndexValue[i - 1] >= localmax) localmax = IndexValue[i - 1];
					else if (IndexValue[i - 1] <= localmin) localmin = IndexValue[i - 1];
					else {
						if (max_flag) {
							localmax = *std::max_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
							max_flag = false;
						}
						if (min_flag) {
							localmin = *std::min_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
							min_flag = false;
						}
					}
				}
				if (localmax == IndexValue[i - n - 1]) max_flag = true;
				if (localmin == IndexValue[i - n - 1]) min_flag = true;
				days++;
			}
		}
	}
	else if (c != 0 && e != 0 && n == 0) {
		//find e-max(min) vector
		std::vector<double> max_vector(position.size());
		std::vector<double> min_vector(position.size());
		bool max_flag = false;
		bool min_flag = false;
		for (unsigned int i = e; i < IndexValue.size(); i++) {
			if (i == e) {
				localmax = *std::max_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
				localmin = *std::min_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
			}
			else {
				if (IndexValue[i] >= localmax) {
					localmax = IndexValue[i];
					max_vector[i] = IndexValue[i];
					min_vector[i] = min_vector[i - 1];
				}
				else if (IndexValue[i] <= localmin) {
					localmin = IndexValue[i];
					min_vector[i] = IndexValue[i];
					max_vector[i] = max_vector[i - 1];
				}
				else {
					max_vector[i] = max_vector[i - 1];
					min_vector[i] = min_vector[i - 1];
					if (max_flag) {
						localmax = *std::max_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
						max_flag = false;
					}
					if (min_flag) {
						localmin = *std::min_element(IndexValue.begin() + i - e, IndexValue.begin() + i);
						min_flag = false;
					}
				}
			}
			if (localmax == IndexValue[i - e]) max_flag = true;
			if (localmin == IndexValue[i - e]) min_flag = true;
		}
		bool Flag = true;
		int days = 100;
		if (b == 0 && d == 0) {
			for (unsigned int i = 2; i < position.size(); i++) {
				if (Flag == false && days == c) {
					Flag = true;
					position[i] = 0;
					days = 100;
					continue;
				}
				if (max_vector[i - 2] != 0 && IndexValue[i - 1] > max_vector[i - 2] && Flag == true) {
					position[i] = 1;
					Flag = false;
					days = 0;
				}
				else if (min_vector[i - 2] != 0 && IndexValue[i - 1] < min_vector[i - 2] && Flag == true) {
					position[i] = -1;
					Flag = false;
					days = 0;
				}
				else position[i] = position[i - 1];
				days++;
			}
		}
		else if (b != 0 && d == 0) {
			for (unsigned int i = 2; i < position.size(); i++) {
				if (Flag == false && days == c) {
					Flag = true;
					position[i] = 0;
					days = 100;
					continue;
				}
				if (max_vector[i - 2] != 0 && IndexValue[i - 1] > (1 + b) * max_vector[i - 2] && Flag == true) {
					position[i] = 1;
					Flag = false;
					days = 0;
				}
				else if (min_vector[i - 2] != 0 && IndexValue[i - 1] < (1 - b) * min_vector[i - 2] && Flag == true) {
					position[i] = -1;
					Flag = false;
					days = 0;
				}
				else position[i] = position[i - 1];
				days++;
			}
		}
		else if (b == 0 && d != 0) {
			for (unsigned int i = d + 1; i < position.size(); i++) {
				double min_signal = *std::min_element(IndexValue.begin() + i - d, IndexValue.begin() + i);
				double max_signal = *std::max_element(IndexValue.begin() + i - d, IndexValue.begin() + i);
				if (Flag == false && days == c) {
					Flag = true;
					position[i] = 0;
					days = 100;
					continue;
				}
				if (max_vector[i - d - 1] != 0 && min_signal > max_vector[i - d - 1] && Flag == true) {
					position[i] = 1;
					Flag = false;
					days = 0;
				}
				else if (min_vector[i - d - 1] != 0 && max_signal < min_vector[i - d - 1] && Flag == true) {
					position[i] = -1;
					Flag = false;
					days = 0;
				}
				else position[i] = position[i - 1];
				days++;
			}
		}
	}
	else std::cout << "c, n, e parameters are not as required." << std::endl;
	return position;
}

std::vector<short> UniverseRules::ChannelRulesPosition(unsigned int n, double x, double b, unsigned int c) const {
	double localmax = IndexValue[0];
	double localmin = IndexValue[0];
	std::vector<short> position(IndexValue.size());
	bool Flag = true;
	int days = 100;
	if (b == 0) {
		for (unsigned int i = n + 1; i < position.size(); i++) {
			localmax = *std::max_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
			localmin = *std::min_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);

			if (Flag == false && days == c) {
				Flag = true;
				position[i] = 0;
				days = 100;
				continue;
			}
			if (Flag == true && localmax - localmin < x * localmax && IndexValue[i - 1] > localmin + localmax * x) {
				Flag = false;
				days = 0;
				position[i] = 1;
			}
			else if (Flag == false && localmax - localmin < x * localmax && IndexValue[i - 1] < localmax - localmax * x) {
				position[i] = -1;
				days = 0;
				Flag = false;
			}
			days++;
		}
	}
	else {
		for (unsigned int i = n + 1; i < position.size(); i++) {
			localmax = *std::max_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);
			localmin = *std::min_element(IndexValue.begin() + i - 1 - n, IndexValue.begin() + i - 1);

			if (Flag == false && days == c) {
				Flag = true;
				position[i] = 0;
				days = 100;
				continue;
			}
			if (Flag == true && localmax - localmin < x * localmax && IndexValue[i - 1] > (1 + b) * (localmin + localmax * x)) {
				Flag = false;
				days = 0;
				position[i] = 1;
			}
			else if (Flag == false && localmax - localmin < x * localmax && IndexValue[i - 1] < (1 - b) * (localmax - localmax * x)) {
				position[i] = -1;
				days = 0;
				Flag = false;
			}
			days++;
		}
	}
	return position;
}
std::vector<short> UniverseRules::MovingAverageRulesPosition(unsigned int n, unsigned int m, double b, unsigned int c, unsigned int d)const {
	std::vector<short> position(IndexValue.size());
	double fast_sum, slow_sum, fast_move, slow_move;
	if (n >= m) {
		std::cout << "n has to be smaller than m";
		return position;
	}
	if (b == 0 && d == 0 && c == 0) {
		for (unsigned int i = m; i < position.size(); i++) {
			if (i == m) {
				fast_sum = std::accumulate(IndexValue.begin() + i - n, IndexValue.begin() + i, 0.0);
				slow_sum = std::accumulate(IndexValue.begin() + i - m, IndexValue.begin() + i, 0.0);
			}
			else {
				fast_sum = fast_sum - IndexValue[i - n - 1] + IndexValue[i - 1];
				slow_sum = slow_sum - IndexValue[i - m - 1] + IndexValue[i - 1];
			}
			fast_move = fast_sum / n;
			slow_move = slow_sum / m;
			if ((position[i - 1] == 0 || position[i -1] == -1) && fast_move > slow_move) position[i] = 1;
			else if ((position[i - 1] == 0 || position[i - 1] == 1) && fast_move < slow_move) position[i] = -1;
			else position[i] = position[i - 1];
		}
	}

	else if (b != 0 && d == 0 && c == 0) {
		for (unsigned int i = m; i < position.size(); i++) {
			if (i == m) {
				fast_sum = std::accumulate(IndexValue.begin() + i - n, IndexValue.begin() + i, 0.0);
				slow_sum = std::accumulate(IndexValue.begin() + i - m, IndexValue.begin() + i, 0.0);
			}
			else {
				fast_sum = fast_sum - IndexValue[i - n - 1] + IndexValue[i - 1];
				slow_sum = slow_sum - IndexValue[i - m - 1] + IndexValue[i - 1];
			}
			fast_move = fast_sum / n;
			slow_move = slow_sum / m;
			if ((position[i - 1] == 0 || position[i - 1] == -1) && fast_move > (1 + b) * slow_move) position[i] = 1;
			else if ((position[i - 1] == 0 || position[i - 1] == 1) && fast_move < (1 - b) * slow_move) position[i] = -1;
			else position[i] = position[i - 1];
		}
	}

	else if (b == 0 && d != 0 && c == 0) {
		double fast_sum_copy, slow_sum_copy;
		for (unsigned int i = m + d - 1; i < position.size(); i++) {
			bool buy_Flag = true;
			bool sell_Flag = true;
			if (i == m + d - 1) {
				fast_sum = std::accumulate(IndexValue.begin() + i - n - d + 1, IndexValue.begin() + i - d + 1, 0.0);
				slow_sum = std::accumulate(IndexValue.begin(), IndexValue.begin() + 2, 0.0);
			}
			else {
				fast_sum = fast_sum - IndexValue[i - n - d] + IndexValue[i - d];
				slow_sum = slow_sum - IndexValue[i - m - d] + IndexValue[i - d];
			}
			fast_sum_copy = fast_sum;
			slow_sum_copy = slow_sum;
			for (unsigned int j = 0; j < d; j++) {
				if (j != 0) {
					fast_sum_copy = fast_sum_copy - IndexValue[i - n - d + j] + IndexValue[i - d + j];
					slow_sum_copy = slow_sum_copy - IndexValue[i - m - d + j] + IndexValue[i - d + j];
				}
				fast_move = fast_sum_copy / n;
				slow_move = slow_sum_copy / m;
				if (fast_move < slow_move) buy_Flag = false;
				else sell_Flag = false;
			}
			if ((position[i - 1] == 0 || position[i - 1] == -1) && buy_Flag) position[i] = 1;
			else if ((position[i - 1] == 0 || position[i - 1] == 1) && sell_Flag) position[i] = -1;
			else position[i] = position[i - 1];
		}
	}

	else if (b == 0 && d == 0 && c != 0) {
		bool Flag = true;
		int days = 100;
		for (unsigned int i = m; i < position.size(); i++) {

			if (Flag == false && days == c) {
				Flag = true;
				position[i] = 0;
				days = 100;
				continue;
			}

			if (i == m) {
				fast_sum = std::accumulate(IndexValue.begin() + i - n, IndexValue.begin() + i, 0.0);
				slow_sum = std::accumulate(IndexValue.begin() + i - m, IndexValue.begin() + i, 0.0);
			}
			else {
				fast_sum = fast_sum - IndexValue[i - n - 1] + IndexValue[i - 1];
				slow_sum = slow_sum - IndexValue[i - m - 1] + IndexValue[i - 1];
			}
			fast_move = fast_sum / n;
			slow_move = slow_sum / m;
			if (Flag && (position[i - 1] == 0 || position[i - 1] == -1) && fast_move > slow_move) {
				position[i] = 1;
				days = 0;
				Flag = false;
			}
			else if (Flag && (position[i - 1] == 0 || position[i - 1] == 1) && fast_move < slow_move) {
				position[i] = -1;
				days = 0;
				Flag = false;
			}
			else position[i] = position[i - 1];

			days++;
		}
	}
	else std::cout << "There can be at most one nonzero of parameters b, d, and c" << std::endl;

	return position;
}

std::vector<double> UniverseRules::getOBV() const {
	std::vector<double> OBV(IndexValue.size());
	for (unsigned int i = 0; i < OBV.size(); i++) {
		if (i == 0) OBV[i] = IndexVolume[i];
		else {
			if (IndexValue[i] >= IndexValue[i - 1]) OBV[i] = OBV[i - 1] + IndexVolume[i];
			else OBV[i] = OBV[i - 1] - IndexVolume[i];
		}
	}
	return OBV;
}
std::vector<short> UniverseRules::OBVRulesPosition(unsigned int n, unsigned int m, double b, unsigned int c, unsigned int d)const {
	std::vector<double> OBV = getOBV();
	std::vector<short> position(OBV.size());
	double fast_sum, slow_sum, fast_move, slow_move;
	if (n >= m) {
		std::cout << "n has to be smaller than m";
		return position;
	}
	if (b == 0 && d == 0 && c == 0) {
		for (unsigned int i = m; i < position.size(); i++) {
			if (i == m) {
				fast_sum = std::accumulate(OBV.begin() + i - n, OBV.begin() + i, 0.0);
				slow_sum = std::accumulate(OBV.begin() + i - m, OBV.begin() + i, 0.0);
			}
			else {
				fast_sum = fast_sum - OBV[i - n - 1] + OBV[i - 1];
				slow_sum = slow_sum - OBV[i - m - 1] + OBV[i - 1];
			}
			fast_move = fast_sum / n;
			slow_move = slow_sum / m;
			if ((position[i - 1] == 0 || position[i - 1] == -1) && fast_move > slow_move) position[i] = 1;
			else if ((position[i - 1] == 0 || position[i - 1] == 1) && fast_move < slow_move) position[i] = -1;
			else position[i] = position[i - 1];
		}
	}

	else if (b != 0 && d == 0 && c == 0) {
		for (unsigned int i = m; i < position.size(); i++) {
			if (i == m) {
				fast_sum = std::accumulate(OBV.begin() + i - n, OBV.begin() + i, 0.0);
				slow_sum = std::accumulate(OBV.begin() + i - m, OBV.begin() + i, 0.0);
			}
			else {
				fast_sum = fast_sum - OBV[i - n - 1] + OBV[i - 1];
				slow_sum = slow_sum - OBV[i - m - 1] + OBV[i - 1];
			}
			fast_move = fast_sum / n;
			slow_move = slow_sum / m;
			if ((position[i - 1] == 0 || position[i - 1] == -1) && fast_move > (1 + b) * slow_move) position[i] = 1;
			else if ((position[i - 1] == 0 || position[i - 1] == 1) && fast_move < (1 - b) * slow_move) position[i] = -1;
			else position[i] = position[i - 1];
		}
	}

	else if (b == 0 && d != 0 && c == 0) {
		double fast_sum_copy, slow_sum_copy;
		for (unsigned int i = m + d - 1; i < position.size(); i++) {
			bool buy_Flag = true;
			bool sell_Flag = true;
			if (i == m + d - 1) {
				fast_sum = std::accumulate(OBV.begin() + i - n - d + 1, OBV.begin() + i - d + 1, 0.0);
				slow_sum = std::accumulate(OBV.begin(), OBV.begin() + 2, 0.0);
			}
			else {
				fast_sum = fast_sum - OBV[i - n - d] + OBV[i - d];
				slow_sum = slow_sum - OBV[i - m - d] + OBV[i - d];
			}
			fast_sum_copy = fast_sum;
			slow_sum_copy = slow_sum;
			for (unsigned int j = 0; j < d; j++) {
				if (j != 0) {
					fast_sum_copy = fast_sum_copy - OBV[i - n - d + j] + OBV[i - d + j];
					slow_sum_copy = slow_sum_copy - OBV[i - m - d + j] + OBV[i - d + j];
				}
				fast_move = fast_sum_copy / n;
				slow_move = slow_sum_copy / m;
				if (fast_move < slow_move) buy_Flag = false;
				else sell_Flag = false;
			}
			if ((position[i - 1] == 0 || position[i - 1] == -1) && buy_Flag) position[i] = 1;
			else if ((position[i - 1] == 0 || position[i - 1] == 1) && sell_Flag) position[i] = -1;
			else position[i] = position[i - 1];
		}
	}

	else if (b == 0 && d == 0 && c != 0) {
		bool Flag = true;
		int days = 100;
		for (unsigned int i = m; i < position.size(); i++) {

			if (Flag == false && days == c) {
				Flag = true;
				position[i] = 0;
				days = 100;
				continue;
			}

			if (i == m) {
				fast_sum = std::accumulate(OBV.begin() + i - n, OBV.begin() + i, 0.0);
				slow_sum = std::accumulate(OBV.begin() + i - m, OBV.begin() + i, 0.0);
			}
			else {
				fast_sum = fast_sum - OBV[i - n - 1] + OBV[i - 1];
				slow_sum = slow_sum - OBV[i - m - 1] + OBV[i - 1];
			}
			fast_move = fast_sum / n;
			slow_move = slow_sum / m;
			if (Flag && (position[i - 1] == 0 || position[i - 1] == -1) && fast_move > slow_move) {
				position[i] = 1;
				days = 0;
				Flag = false;
			}
			else if (Flag && (position[i - 1] == 0 || position[i - 1] == 1) && fast_move < slow_move) {
				position[i] = -1;
				days = 0;
				Flag = false;
			}
			else position[i] = position[i - 1];

			days++;
		}
	}
	else std::cout << "There can be at most one nonzero of parameters b, d, and c" << std::endl;

	return position;
}

UniverseRules::~UniverseRules()
{
}
