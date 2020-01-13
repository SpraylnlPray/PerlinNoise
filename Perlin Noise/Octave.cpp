#include "pch.h"
#include "Octave.h"

void Octave::create()
{
	int signalIndex = 0;
	float currentTime = _signalTimes[signalIndex];
	float currentValue = _signalValues[signalIndex];
	double val;

	save(_signalValues[0]); 

	for (; ; )
	{
		currentTime += _stepSize; // erhöhen des Zeitcounter um den Zeitschritt

		if (_writeIndex >= _totalValCount)
			return;

		auto t = (currentTime - _signalTimes[signalIndex]) / (_signalTimes[signalIndex + 1] - _signalTimes[signalIndex]);
		val = currentValue + (_signalValues[signalIndex + 1] - currentValue) * S_t(t); // interpolation Value with S curve function

		save(val);

		// Wenn ein Signalpunkt überschritten wird, wird aktuelles Signal auf nächstes gesetzt
		if (currentTime >= _signalTimes[signalIndex + 1])
		{
			signalIndex++;
			currentValue = _signalValues[signalIndex];
			currentTime = _signalTimes[signalIndex];
		}
	}
}

bool Octave::createStructures()
{
	_signalCount = std::pow(2, _rank - 1) + 1; // Anzahl an Fixpunkten
	_signalTimes = new float[_signalCount * _intervals];
	_signalValues = new float[_signalCount * _intervals];
	return false;
}

bool Octave::initialize()
{
	this->_stream.open(std::to_string(_rank) + ".txt"); // initialisiere Stream zum speichern von Werten

	_totalValCount = _intervals / _stepSize;
	_valsToWrite = new float[_totalValCount];

	float signalDiffTime = _rank == 1 ? 1 : 1.f / ((float)_signalCount - 1); // wie viel Zeit vergeht zwischen den einzelnen Fixpunkten

	_signalTimes[0] = 0;
	_signalTimes[_signalCount * _intervals - 1] = 1; // t von erster und letzter Fixpunkt stehen fest

	for (int i = 1; i < _signalCount * _intervals - 1; i++) // speichern der Zeiten von Fixpunkten
	{
		_signalTimes[i] = signalDiffTime;
		signalDiffTime += signalDiffTime;
	}

	for (int i = 0; i < _signalCount * _intervals; i++) 	// erstellen der Fixwerte
	{
		float signalValue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // random from 0 to 1 inclusive
		_signalValues[i] = signalValue;
	}

	return false;
}

bool Octave::write()
{
	std::cout << "Octave with rank " << _rank << " writes values" << std::endl;
	for (int i = 0; i < _writeIndex; i++)
	{
		std::string number = std::to_string(_valsToWrite[i]); // Im skript O
		size_t found = number.find(".");
		if (found != std::string::npos) // only for google tables
			number[found] = ',';

		_stream << number << std::endl;
	}

	return false;
}

bool Octave::save(double value)
{
	_valsToWrite[_writeIndex] = value;
	_writeIndex++;
	return false;
}
