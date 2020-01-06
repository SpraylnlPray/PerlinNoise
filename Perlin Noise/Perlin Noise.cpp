#include "pch.h"
#include <cmath>
#include <map>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

double S_t(double t)
{
	return pow(t, 3) * (10 + t * (-15 + 6 * t));
}

void PerlinNoise(int rang);
void lerp(std::map<float, float> timeValuePairs, float* signaltimes, float timediff);

int main()
{
	PerlinNoise(2);
}

void PerlinNoise(int rang)
{
	int signalCount = (int)std::pow(2, rang - 1) + 1; // Anzahl an Fixpunkten
	float* signalTimes = new float[signalCount];
	signalTimes[0] = 0;
	signalTimes[signalCount - 1] = 1; // t erster und letzter Fixpunkt stehen fest
	float timeDiff = rang == 1 ? 1 : 1.f / ((float)signalCount - 1); // wie viel Zeit vergeht zwischen den einzelnen Fixpunkten
	float signalTime = timeDiff;
	for (int i = 1; i < signalCount - 1; i++) // speichern der Zeiten von Fixpunkten
	{
		signalTimes[i] = signalTime;
		signalTime += timeDiff;
		std::cout << signalTimes[i] << std::endl;
	}
	std::map<float, float> timeValuePairs;

	srand(time(0));
	// erstellen von Wertepaaren t und s, für zeitpunkt t und wert s(t)
	for (int i = 0; i < signalCount; i++)
	{
		float signalValue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // random from 0 to 1 inclusive
		timeValuePairs.emplace(signalTimes[i], signalValue);
	}

	lerp(timeValuePairs, signalTimes, timeDiff);
	std::cout << std::endl;
}

void lerp(std::map<float, float> timeValuePairs, float* signaltimes, float timediff)
{
	std::ofstream stream;
	stream.open("test.txt");
	int timeIndex = 0;
	double timeStep = 0.01; // Zeitschritt für Berechnung
	float currentTime = signaltimes[timeIndex]; // Beginn bei 0
	float currentSignalValue = timeValuePairs.at(currentTime); // Wert beim aktuellen Signal

	std::string number = std::to_string(std::abs(currentSignalValue)); // Im skript O
	size_t found = number.find(".");
	if (found != std::string::npos)
		number[found] = ',';

	stream << number << std::endl;

	int nextSignalIndex = 1;
	float nextSignalValue = timeValuePairs.at(signaltimes[nextSignalIndex]); // todo: lesbarkeit verbessern
	for (currentTime; currentTime < 1; )
	{		
		currentTime += timeStep; // erhöhen des Zeitcounter um den Zeitschritt
		
		if (currentTime >= 1)
			return;

		auto t = (currentTime - signaltimes[timeIndex]) / (signaltimes[nextSignalIndex] - signaltimes[timeIndex]);
		number = std::to_string(currentSignalValue + (nextSignalValue - currentSignalValue) * S_t(t));

		size_t found = number.find(".");
		if (found != std::string::npos)
			number[found] = ',';
		
		stream << number << std::endl;

		// Wenn ein Signalpunkt überschritten wird, wird das aktuell nächste Signal auf das aktuelle und das übernächste auf das nächste gesetzt
		if (currentTime >= signaltimes[nextSignalIndex])
		{
			timeIndex++;
			currentSignalValue = timeValuePairs.at(signaltimes[nextSignalIndex]);
			nextSignalIndex++;
			nextSignalValue = timeValuePairs.at(signaltimes[nextSignalIndex]);
		}
	}
	stream.close();
}

