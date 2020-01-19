#pragma once
#include <tuple>
class Signal
{
public:
	Signal() {}

	void setCoords(double x, double y)
	{
		_coordinates = std::make_tuple(x, y);
	}

	void setValue(float val)
	{
		_signalValue = val;
	}

	double X()
	{
		return std::get<0>(_coordinates);
	}

	double Y()
	{
		return std::get<1>(_coordinates);
	}

	double Value()
	{
		return _signalValue;
	}

private:
	std::tuple<double, double> _coordinates;
	float _signalValue;
};

