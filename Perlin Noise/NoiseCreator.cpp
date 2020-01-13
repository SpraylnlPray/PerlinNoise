#include "pch.h"
#include "NoiseCreator.h"

NoiseCreator::NoiseCreator(int octaveCount, int secs)
{
	std::cout << "Initialized Noise Creator with " << octaveCount << " octaves for " << secs << " seconds." << std::endl;
	srand(time(0)); // random initialisieren
	this->_octaveCount = octaveCount;
	_octaves = new Octave * [octaveCount];
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
}
