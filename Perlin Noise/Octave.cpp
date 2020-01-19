#include "pch.h"
#include "Octave.h"
#include "NoiseCreator.h"

void Octave::Create()
{
	int signalXIndex = 0;
	int signalYIndex = 0;
	auto currentSignal = _signals[signalXIndex][signalYIndex];
	float valY;
	float valX;
	float val;
	int line = 0;

	for (float x = 0; x <= _intervals; x += _tickSize) // Problem: Wenn in einem Quadranten die Mittellinie überschritten wird, muss statt dem Startsignal das gegenüber für 
		// die Interpolation gewählt werden. --> Algorithmus um herauszufinden welche Signal beachtet werden müssen.
	{
		for (float y = 0; y <= _intervals; y += _tickSize)
		{
			float t_X = (x - currentSignal.X()) / (_signals[signalXIndex + 1][signalYIndex].X() - currentSignal.X());
			valX = (currentSignal.Value() + (_signals[signalXIndex + 1][signalYIndex].Value() - currentSignal.Value()) * S_t(t_X)); // interpolation on x with S curve function
			float t_Y = (y - currentSignal.Y()) / (_signals[signalXIndex][signalYIndex + 1].Y() - currentSignal.Y());
			valY = (currentSignal.Value() + (_signals[signalXIndex][signalYIndex + 1].Value() - currentSignal.Value()) * S_t(t_Y)); // interpolation on y

			val = valX + valY - currentSignal.Value();

			save(line, val);

			if (y >= _signals[signalXIndex][signalYIndex + 1].Y())
			{
				signalYIndex++;
				currentSignal = _signals[signalXIndex][signalYIndex];
				y = _signals[signalXIndex][signalYIndex].Y();
			}
		}
		if (x >= _signals[signalXIndex + 1][signalYIndex].X()) // if x coordinate reaches next signal
		{
			signalXIndex++; // increase x index
			currentSignal = _signals[signalXIndex][signalYIndex]; // and set new current signal
			x = _signals[signalXIndex][signalYIndex].X();
		}
		signalYIndex = 0;
		currentSignal = _signals[signalXIndex][signalYIndex];
		_writeIndex = 0;
		line++;
	}
}

bool Octave::createStructures()
{
	_signalCount = (int)std::pow(2, _rank - 1) + 1; // Anzahl an Fixpunkten
	_signals = new Signal* [_signalCount * _intervals] { nullptr };
	for (int i = 0; i < _signalCount * _intervals; i++)
	{
		_signals[i] = new Signal();
	}
	return false;
}

bool Octave::initialize()
{
	this->_fileName = std::to_string(_rank) + ".txt";
	this->_stream.open(_fileName);

	_totalValCount = _intervals / _tickSize; // Anzahl Werte die geschrieben wird
	_valsToWrite = new float[_totalValCount];

	float signalDiffTime = _rank == 1 ? 1 : 1.f / ((float)_signalCount - 1); // wie viel Zeit vergeht zwischen den einzelnen Fixpunkten

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::minstd_rand0 g1(seed1);
	float signalValue;

	//float randomVals[9] = {0.f, 0.3f, 0.6f, 0.3f, 0.6f, 0.9f, 0.6, 0.9f, 0.6f};
	//int randomIndex = 0;

	for (int y = 0; y < _signalCount * _intervals - 1; y++)
	{
		for (int x = 0; x < _signalCount * _intervals - 1; x++)
		{
			_signals[x][y].setCoords(x * signalDiffTime, y * signalDiffTime);
			signalValue = g1();
			signalValue = signalValue / g1.max();
			//signalValue = randomVals[randomIndex];
			_signals[x][y].setValue(signalValue);
			//randomIndex++;
		}

	}

	return false;
}

bool Octave::Write()
{
	std::cout << "Octave with rank " << _rank << " writing values" << std::endl;
	
	for (int i = 0; i < _writeIndex; i++)
	{
		_stream << _valsToWrite[i] << std::endl;
	}

	return false;
}

bool Octave::save(int line, float value)
{
	_creator->Save(line, _writeIndex, value);
	// _valsToWrite[_writeIndex] = value;
	_writeIndex++;
	return false;
}
