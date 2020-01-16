#include "pch.h"
#include "Octave.h"

void Octave::Create()
{
	int signalIndex = 0;
	float currentTime = _signalTimes[signalIndex];
	float currentValue = _signalValues[signalIndex];
	double val;

	save(_signalValues[0]); 

	while(true)
	{
		currentTime += _tickSize; // erhöhen des Zeitcounter um den Zeitschritt

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
	srand(time(0)); // random initialisieren
	this->_fileName = std::to_string(_rank) + ".txt";
	this->_stream.open(_fileName); // initialisiere Stream zum speichern von Werten

	_totalValCount = _intervals / _tickSize;
	_valsToWrite = new float[_totalValCount];

	float signalDiffTime = _rank == 1 ? 1 : 1.f / ((float)_signalCount - 1); // wie viel Zeit vergeht zwischen den einzelnen Fixpunkten

	_signalTimes[0] = 0;
	_signalTimes[_signalCount * _intervals - 1] = 1; // t von erster und letzter Fixpunkt stehen fest

	for (int i = 1; i < _signalCount * _intervals - 1; i++) // speichern der Zeiten von Fixpunkten
	{
		_signalTimes[i] = _signalTimes[i - 1] + signalDiffTime;
	}

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::minstd_rand0 g1(seed1);

	for (int i = 0; i < _signalCount * _intervals; i++) 	// erstellen der Fixwerte
	{
		float signalValue = g1();
		_signalValues[i] = signalValue / g1.max();
	}

	return false;
}

bool Octave::Write()
{
	std::cout << "Octave with rank " << _rank << " writes values" << std::endl;
	for (int i = 0; i < _writeIndex; i++)
	{
		_stream << _valsToWrite[i] << std::endl;
	}

	return false;
}

bool Octave::save(double value)
{
	_valsToWrite[_writeIndex] = value;
	_writeIndex++;
	return false;
}
