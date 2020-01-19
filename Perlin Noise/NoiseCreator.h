#pragma once
#include <iostream>
#include <ctime>
#include "Octave.h"


class NoiseCreator
{
public:
	NoiseCreator(int octaveCount, int intervals);
	void Create();
	void WriteNames();
	void WriteConfig();
	void Save(int row, int index, float value);

private:
	void weigh();
	bool writeValues();
	void printVals();

	int _totalValCount; // How many values are being written in total
	int _octaveCount;
	int _seconds;

	float** _memory;
	float* _weighed;
	float _timeStep = 0.1f; // Zeitschritt für Berechnung

	Octave** _octaves;

	std::ofstream _stream;

	std::string _averageFileName = "average.txt";
	std::string _nameFileName = "names.txt";
	std::string _configFileName = "config.txt";
};

