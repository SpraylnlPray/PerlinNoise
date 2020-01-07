#include "pch.h"
#include "NoiseCreator.h"

NoiseCreator::NoiseCreator(int rang, int secs)
{
	this->intervals = secs;
	SetInitVals(rang);
	Initialize();
}

void NoiseCreator::Create()
{
	int index = 0;
	float timeStep = 0.001f; // Zeitschritt für Berechnung
	float currentTime = signalTimes[index];
	float currentValue = signalValues[index];
	double val;

	formatAndWrite(signalValues[0]);

	for ( ; ; )
	{
		currentTime += timeStep; // erhöhen des Zeitcounter um den Zeitschritt

		if (currentTime >= intervals)
			return;

		auto t = (currentTime - signalTimes[index]) / (signalTimes[index + 1] - signalTimes[index]);
		val = currentValue + (signalValues[index + 1] - currentValue) * S_t(t);

		formatAndWrite(val);

		// Wenn ein Signalpunkt überschritten wird, wird aktuelles Signal auf nächstes gesetzt
		if (currentTime >= signalTimes[index + 1])
		{
			index++;
			currentValue = signalValues[index];
			currentTime = signalTimes[index];
		}
	}
}

void NoiseCreator::SetInitVals(int rang)
{
	signalCount = (int)std::pow(2, rang - 1) + 1; // Anzahl an Fixpunkten
	signalTimes = new float[signalCount * intervals];
	signalValues = new float[signalCount * intervals];

	signalTimes[0] = 0;
	signalTimes[signalCount * intervals - 1] = 1; // t erster und letzter Fixpunkt stehen fest

	stepSize = rang == 1 ? 1 : 1.f / ((float)signalCount - 1); // wie viel Zeit vergeht zwischen den einzelnen Fixpunkten
}

void NoiseCreator::Initialize()
{
	this->stream.open("test.txt"); // initialisiere Stream zum speichern von Werten

	float signalTime = stepSize;
	for (int i = 1; i < signalCount * intervals - 1; i++) // speichern der Zeiten von Fixpunkten
	{
		signalTimes[i] = signalTime;
		signalTime += stepSize;
	}

	srand(time(0)); // random initialisieren

	for (int i = 0; i < signalCount * intervals; i++) 	// erstellen der Fixwerte
	{
		float signalValue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // random from 0 to 1 inclusive
		signalValues[i] = signalValue;
	}
}
