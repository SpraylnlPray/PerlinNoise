#pragma once
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

class Octave
{
public:
	Octave(int intervals, float stepSize, int rank) 
		: _intervals(intervals), _stepSize(stepSize), _rank(rank)
	{
		std::cout << "Initialized Octave with rank " << rank << std::endl;
		createStructures();
		initialize();
	}

	void create();
	bool write();

private:
	int _intervals; // How many seconds 
	int _rank;
	int _signalCount; // How many fix points does the octave have
	int _writeIndex = 0;
	int _totalValCount; // How many values are being written in total

	float* _signalTimes = nullptr; 
	float* _signalValues = nullptr;
	float* _valsToWrite;
	float _stepSize; // How big are the steps between two interpolations

	double S_t(double t)
	{
		return pow(t, 3) * (10 + t * (-15 + 6 * t));
	}

	bool createStructures();
	bool initialize();
	bool save(double value);

	std::ofstream _stream;
};

