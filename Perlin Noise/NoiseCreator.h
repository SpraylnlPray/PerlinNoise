#pragma once
#include <iostream>
#include <ctime>
#include "Octave.h"

class NoiseCreator
{
public:
	NoiseCreator(int octaveCount, int intervals);
	void Create();

private:
	void Weigh();
	bool Write();

	int _totalValCount; // How many values are being written in total
	int _octaveCount;

	float* _averaged;
	float _timeStep = 0.01f; // Zeitschritt für Berechnung

	Octave** _octaves;

	std::ofstream _stream;
};

