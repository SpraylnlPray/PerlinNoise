#pragma once
#include <cmath>
#include <fstream>
#include <ctime>
#include <string>

class NoiseCreator
{
public:
	NoiseCreator(int rang, int intervals);
	void Create();

private:
	void SetInitVals(int rang);
	void Initialize();
	double S_t(double t)
	{
		return pow(t, 3) * (10 + t * (-15 + 6 * t));
	}
	void formatAndWrite(double value)
	{
		std::string number = std::to_string(value); // Im skript O
		size_t found = number.find(".");
		if (found != std::string::npos) // only for google tables
			number[found] = ',';

		stream << number << std::endl;
	}


	std::ofstream stream;
	float* signalTimes = nullptr;
	float* signalValues = nullptr;
	float stepSize = 0;
	int signalCount = 0;
	int intervals;
};

