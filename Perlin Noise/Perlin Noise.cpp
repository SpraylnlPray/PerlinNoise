#include "pch.h"
#include <cmath>
#include <map>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

void PerlinNoise(int rang);
void lerp(std::map<float, float> timeValuePairs, float* signaltimes, float timediff);
float generalisedLogisticFunc(float A, float K, float C, float B, float t, float M, float v);

//	A: Lower asymptote
//	K: The upper asymptote when C = 1
//	B: Growth rate
//	C: Typically takes a value of 1
//	t: time
//	M: starting time - which will for now be 0
//	v > 0: affects near which asymptote max growth occurs - leave at 0.5

int main()
{
	std::ofstream stream;
	stream.open("test2.txt");
	float A = 0.251;
	float K = 0.762;
	float B = 2.5;
	float v = 0.5;
	float M = 0;
	float C = 1;
	float value;
	for (float t = -1.5; t < 2; t += 0.01)
	{
		value = generalisedLogisticFunc(A, K, C, B, t, M, v);
		std::string number = std::to_string(std::abs(value));
		size_t found = number.find(".");
		if (found != std::string::npos)
			number[found] = ',';
		stream << number << std::endl;
	}

	// PerlinNoise(2);
}

void PerlinNoise(int rang)
{
	int signalCount = (int)std::pow(2, rang - 1) + 1; // Anzahl an Fixpunkten
	float* signalTimes = new float[signalCount];
	signalTimes[0] = 0;
	signalTimes[signalCount - 1] = 1; // t erster und letzter Fixpunkt stehen fest
	float timeDiff = rang == 1 ? 1 : 1.f / ((float)signalCount - 1); // wie viel Zeit vergeht zwischen den einzelnen Fixpunkten
	float signalTime = timeDiff;
	for (int i = 1; i < signalCount - 1; i++)
	{
		signalTimes[i] = signalTime; // festlegen der Zeitpunkte
		signalTime += timeDiff;
		std::cout << signalTimes[i] << std::endl;
	}
	std::map<float, float> timeValuePairs;

	srand(time(0));
	// bestimmen der random werte zwischen 0 und 1 für alle Punkte in signals
	for (int i = 0; i < signalCount; i++)
	{
		float signalValue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		timeValuePairs.emplace(signalTimes[i], signalValue);
	}

	lerp(timeValuePairs, signalTimes, timeDiff);
	std::cout << std::endl;
}

void lerp(std::map<float, float> timeValuePairs, float* signaltimes, float timediff)
{
	std::ofstream stream;
	stream.open("test.txt");
	float timeStep = 0.01; // Zeitschritt für Berechnung // todo: wird nicht als 0.01 gespeichert???
	float currentTime = signaltimes[0]; // Beginn bei 0
	float currentSignalValue = timeValuePairs.at(currentTime); // Wert beim aktuellen Signal

	std::string number = std::to_string(std::abs(currentSignalValue));
	size_t found = number.find(".");
	if (found != std::string::npos)
		number[found] = ',';

	stream << number << std::endl;

	int nextSignalIndex = 1;
	float nextSignalValue = timeValuePairs.at(signaltimes[nextSignalIndex]);
	for (currentTime; currentTime < 1; )
	{
		currentTime += timeStep; // erhöhen des Zeitcounter um den Zeitschritt
		if (currentTime >= 1)
			return;
		// Prozentuale Gewichtung des Wertes, je nachdem wie nah er an einem Signal ist
		float weight = currentTime / timediff;
		float leftWeighed = currentSignalValue * (1 - weight);
		float rightWeighed = nextSignalValue * weight;
		float newValue;
		if (currentSignalValue < nextSignalValue) // todo: umdrehen
			newValue = leftWeighed + rightWeighed;
		else
			newValue = leftWeighed - rightWeighed;

		number = std::to_string(std::abs(newValue));
		size_t found = number.find(".");
		if (found != std::string::npos)
			number[found] = ',';

		stream << number << std::endl;
		// Wenn ein Signalpunkt überschritten wird, wird das aktuell nächste Signal auf das aktuelle und das übernächste auf das nächste gesetzt
		if (currentTime >= signaltimes[nextSignalIndex])
		{
			currentSignalValue = timeValuePairs.at(signaltimes[nextSignalIndex]);
			nextSignalIndex++;
			nextSignalValue = timeValuePairs.at(signaltimes[nextSignalIndex]);
		}
	}
	stream.close();
}

//	A: Lower asymptote
//	K: The upper asymptote when C = 1
//	B: Growth rate
//	C: Typically takes a value of 1
//	t: time
//	M: starting time - which will for now be 0
//	v > 0: affects near which asymptote max growth occurs - leave at 0.5

float generalisedLogisticFunc(float A, float K, float C, float B, float t, float M, float v)
{
	const double e = std::exp(1.0);
	float Y = A + (K - A) / std::pow(C + std::pow(e, -B * (t - M)), 1 / v);
	return Y;
}