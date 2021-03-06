#include "pch.h"
#include "NoiseCreator.h"

NoiseCreator::NoiseCreator(int octaveCount, int secs)
{
	std::cout << "Initialized Noise Creator with " << octaveCount << " octaves for " << secs << " seconds." << std::endl;

	srand(time(0)); // random initialisieren

	this->_octaveCount = octaveCount;
	_octaves = new Octave* [octaveCount];

	_seconds = secs;
	_totalValCount = _seconds / _timeStep;
	_weighed = new float[_totalValCount] {};
	_memory = new float* [_totalValCount] {};
	for (int i = 0; i < _totalValCount; i++)
		_memory[i] = new float[_totalValCount];

	for (int i = 0; i < octaveCount; i++)
		_octaves[i] = new Octave(secs, this->_timeStep, i + 1, this);
}

void NoiseCreator::Create()
{
	for (int i = 0; i < _octaveCount; i++)
	{
		_octaves[i]->Create();
		// _octaves[i]->Write();
	}
	printVals();
	//weigh();
	//writeValues();
}

void NoiseCreator::WriteNames()
{
	std::cout << "Writing filenames" << std::endl;
	_stream.open(this->_nameFileName);
	for (int i = 0; i < _octaveCount; i++)
		_stream << std::to_string(i + 1) + ".txt" << std::endl;
	_stream << this->_averageFileName << std::endl;
	if (_stream.is_open())
		_stream.close();
}

void NoiseCreator::WriteConfig()
{
	std::cout << "Writing config" << std::endl;
	_stream.open(this->_configFileName);
	_stream << _timeStep << std::endl;
	_stream << _seconds << std::endl;
	if (_stream.is_open())
		_stream.close();
}

void NoiseCreator::weigh()
{
	std::cout << "Noise Creator creating weigh over all octaves" << std::endl;
	for (int octaveIndex = 0; octaveIndex < _octaveCount; octaveIndex++)
	{
		float multiplier = std::pow(0.5, octaveIndex + 1);
		for (int valIndex = 0; valIndex < _totalValCount; valIndex++)
		{
			_weighed[valIndex] += multiplier * (*_octaves[octaveIndex])[valIndex];
		}
	}
}

bool NoiseCreator::writeValues()
{
	std::cout << "Noise Creator writing weighed noise" << std::endl;
	this->_stream.open(_averageFileName);
	for (int i = 0; i < _totalValCount; i++)
	{
		_stream << _weighed[i] << std::endl;
	}
	if (_stream.is_open())
		_stream.close();

	return false;
}

void NoiseCreator::printVals()
{
	//for (int i = 0; i < _totalValCount; i++)
	//{
	//	for (int j = 0; j < _totalValCount; j++)
	//	{
	//		std::cout << _memory[i][j] << " ";
	//	}
	//	std::cout << std::endl;
	//}

	std::cout << "writing xy values" << std::endl;
	this->_stream.open("xy.txt");
	for (int i = 0; i < _totalValCount; i++)
	{
		for (int j = 0; j < _totalValCount; j++)
		{
			_stream << _memory[i][j] << " ";
		}
		_stream << std::endl;
	}
	if (_stream.is_open())
		_stream.close();

}

void NoiseCreator::Save(int row, int index, float value)
{
	_memory[row][index] = value;
}
