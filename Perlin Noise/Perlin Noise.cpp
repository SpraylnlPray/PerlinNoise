#include "pch.h"
#include "NoiseCreator.h"

int main()
{
	NoiseCreator creator(3, 3);
	creator.Create();
	creator.WriteNames();
	creator.WriteConfig();
}
