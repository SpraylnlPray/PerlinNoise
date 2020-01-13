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

	int _octaveCount;
	float _timeStep = 0.01f; // Zeitschritt f�r Berechnung
	Octave** _octaves;
};

