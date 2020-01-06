#include "pch.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

std::ofstream stream;

double S_t(double t)
{
	return pow(t, 3) * (10 + t * (-15 + 6 * t));
}

void formatAndWrite(double value)
{

	std::string number = std::to_string(value); // Im skript O
	size_t found = number.find(".");
	if (found != std::string::npos) // only for google tables
		number[found] = ',';

	stream << number << std::endl;
}

void PerlinNoise(int rang);
void lerp(float* signalvalues, float* signaltimes, float timediff);

int main()
{
	try
	{
		stream.open("test.txt");
		PerlinNoise(3);
		stream.close();
	}
	catch (const std::exception&)
	{
		if (stream.is_open())
			stream.close();
	}
}

void PerlinNoise(int rang)
{
	int signalCount = (int)std::pow(2, rang - 1) + 1; // Anzahl an Fixpunkten
	float* signalTimes = new float[signalCount];
	float* signalValues = new float[signalCount];

	signalTimes[0] = 0;
	signalTimes[signalCount - 1] = 1; // t erster und letzter Fixpunkt stehen fest

	float stepSize = rang == 1 ? 1 : 1.f / ((float)signalCount - 1); // wie viel Zeit vergeht zwischen den einzelnen Fixpunkten
	float signalTime = stepSize;
	for (int i = 1; i < signalCount - 1; i++) // speichern der Zeiten von Fixpunkten
	{
		signalTimes[i] = signalTime;
		signalTime += stepSize;
	}

	srand(time(0));
	// erstellen der Fixwerte
	for (int i = 0; i < signalCount; i++)
	{
		float signalValue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // random from 0 to 1 inclusive
		signalValues[i] = signalValue;
	}

	lerp(signalValues, signalTimes, stepSize); // interpolieren zwischen den Fixwerten
	std::cout << std::endl;
}

void lerp(float* signalvalues, float* signaltimes, float timediff)
{
	int index = 0;
	float timeStep = 0.01f; // Zeitschritt für Berechnung
	float currentTime = signaltimes[index];
	float currentValue = signalvalues[index];
	double val;

	formatAndWrite(signalvalues[0]);

	for ( ; ; )
	{	
		currentTime += timeStep; // erhöhen des Zeitcounter um den Zeitschritt
		
		if (currentTime >= 1)
			return;

		auto t = (currentTime - signaltimes[index]) / (signaltimes[index + 1] - signaltimes[index]);
		val = currentValue + (signalvalues[index + 1] - currentValue) * S_t(t);

		formatAndWrite(val);

		// Wenn ein Signalpunkt überschritten wird, wird aktuelles Signal auf nächstes gesetzt
		if (currentTime >= signaltimes[index + 1])
		{
			index++;
			currentValue = signalvalues[index];
			currentTime = signaltimes[index];
		}
	}
}
