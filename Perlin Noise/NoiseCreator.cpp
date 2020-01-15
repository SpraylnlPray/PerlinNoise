#include "pch.h"
#include "NoiseCreator.h"

NoiseCreator::NoiseCreator(int octaveCount, int secs)
{
	std::cout << "Initialized Noise Creator with " << octaveCount << " octaves for " << secs << " seconds." << std::endl;

	this->_stream.open("average.txt"); // initialisiere Stream zum speichern von Werten

	srand(time(0)); // random initialisieren

	this->_octaveCount = octaveCount;

	_octaves = new Octave* [octaveCount];

	_totalValCount = secs / _timeStep;
	_averaged = new float[_totalValCount] {};

	for (int i = 0; i < octaveCount; i++)
		_octaves[i] = new Octave(secs, this->_timeStep, i + 1);
}

void NoiseCreator::Create()
{
	for (int i = 0; i < _octaveCount; i++)
	{
		_octaves[i]->create();
		_octaves[i]->write();
	}

	Weigh();
	Write();
}

void NoiseCreator::Weigh()
{
	std::cout << "Noise Creator creating weigh over all octaves" << std::endl;
	for (int octaveIndex = 0; octaveIndex < _octaveCount; octaveIndex++)
	{
		float multiplier = std::pow(0.5, octaveIndex + 1);
		for (int valIndex = 0; valIndex < _totalValCount; valIndex++)
		{
			_averaged[valIndex] += multiplier * (*_octaves[octaveIndex])[valIndex];
		}
	}
}

bool NoiseCreator::Write()
{
	std::cout << "Noise Creator writing weighed noise" << std::endl;
	for (int i = 0; i < _totalValCount; i++)
	{
		std::string number = std::to_string(_averaged[i]); // Im skript O
		size_t found = number.find(".");
		if (found != std::string::npos) // only for google tables
			number[found] = ',';

		_stream << number << std::endl;
	}

	return false;
}
