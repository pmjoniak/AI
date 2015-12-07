#include "Symulation.h"
#include <cstdlib>

void Symulation::update(double delta_time)
{
	//losowe zdarzenia
	time += delta_time;
	if (time > 24 * 60 * 60)
		time = 0;

	double delta_time_h = delta_time / (60 * 60);

	if (heating)
		t_in += 6*delta_time_h;
	if (cooling)
		t_in -= 6*delta_time_h;

	if (heating)
		energy_consumption += delta_time_h;
	if (cooling)
		energy_consumption += delta_time_h;

	if (window_opened)
		t_in += (t_out - t_in)*delta_time_h / 2;
	else
		t_in += (t_out - t_in)*delta_time_h / 20;

	day = time > 12 * 60 * 60;

	//rain
	if (!raining)
	{
		if ((rand() % 10000) / 10000.0f < 0.1*delta_time_h)
		{
			raining = true;
			sun = false;
			since_raining_start = 0;
		}
	}
	else
	{
		since_raining_start += delta_time;
		if ((rand() % 10000) / 10000.0f < (since_raining_start / (60 * 60)) / 20)
		{
			raining = false;
		}
	}

	if (!day)
		sun = false;

	if (!sun && day && !raining)
	{
		if ((rand() % 10000) / 10000.0f < 0.4*delta_time_h)
		{
			sun = true;
			since_sun_start = 0;
		}
	}
	if (sun)
	{
		since_sun_start += delta_time;
		if ((rand() % 10000) / 10000.0f < (since_sun_start / (60 * 60)) / 40)
			sun = false;
	}
}

float Symulation::getEnergyConsumption()
{
	return energy_consumption;
}

void Symulation::forceRain(bool state)
{
	raining = state;
	if (raining) since_raining_start = 0;
}

float Symulation::getInsideTemerature()
{
	return t_in;
}

float Symulation::getOutsideTemerarure()
{
	return t_out;
}

bool Symulation::isRaining()
{
	return raining;
}

bool Symulation::isDay()
{
	return day;
}

float Symulation::getDayPercentage()
{
	return time / (24 * 60 * 60);
}

bool Symulation::isWindowOpen()
{
	return window_opened;
}

bool Symulation::isHeating()
{
	return heating;
}

bool Symulation::isCooling()
{
	return cooling;
}

void Symulation::closeWindow()
{
	window_opened = false;
}

void Symulation::openWindow()
{
	window_opened = true;
}

void Symulation::stoveOn()
{
	heating = true;
}

void Symulation::stoveOff()
{
	heating = false;
}

bool Symulation::isShutterOpen()
{
	return shutters_opened;
}

void Symulation::closeShutters()
{
	shutters_opened = false;
}

void Symulation::openShutters()
{
	shutters_opened = true;
}

bool Symulation::isSun()
{
	return sun;
}


void Symulation::forceSun(bool state)
{
	if (day && !raining)
		sun = state;
}

void Symulation::forceTempOut(float temp)
{
	t_out = temp;
}

void Symulation::ACOn()
{
	cooling = true;
}

void Symulation::ACOff()
{
	cooling = false;
}