#include "pch.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

std::ofstream stream;
float* signalTimes = nullptr;
float* signalValues = nullptr;
float stepSize = 0;
int signalCount = 0;

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

void PerlinNoise(int rang, int secs);
void SetInitVals(int rang, int secs);
void Interpolate(int secs);

int main()
{
	try
	{
		stream.open("test.txt");
		PerlinNoise(2, 1);
		stream.close();
	}
	catch (const std::exception&)
	{
		if (stream.is_open())
			stream.close();
	}
}

void PerlinNoise(int rang, int secs)
{
	SetInitVals(rang, secs);

	float signalTime = stepSize;
	for (int i = 1; i < signalCount * secs - 1; i++) // speichern der Zeiten von Fixpunkten
	{
		signalTimes[i] = signalTime;
		signalTime += stepSize;
	}

	srand(time(0));
	// erstellen der Fixwerte
	for (int i = 0; i < signalCount * secs; i++)
	{
		float signalValue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // random from 0 to 1 inclusive
		signalValues[i] = signalValue;
	}

	Interpolate(secs); // interpolieren zwischen den Fixwerten
}

void SetInitVals(int rang, int secs)
{
	signalCount = (int)std::pow(2, rang - 1) + 1; // Anzahl an Fixpunkten
	signalTimes = new float[signalCount * secs];
	signalValues = new float[signalCount * secs];

	signalTimes[0] = 0;
	signalTimes[signalCount * secs - 1] = 1; // t erster und letzter Fixpunkt stehen fest

	stepSize = rang == 1 ? 1 : 1.f / ((float)signalCount - 1); // wie viel Zeit vergeht zwischen den einzelnen Fixpunkten
}

void Interpolate(int totaltime)
{
	int index = 0;
	float timeStep = 0.01f; // Zeitschritt für Berechnung
	float currentTime = signalTimes[index];
	float currentValue = signalValues[index];
	double val;

	formatAndWrite(signalValues[0]);

	for ( ; ; )
	{	
		currentTime += timeStep; // erhöhen des Zeitcounter um den Zeitschritt
		
		if (currentTime >= totaltime)
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
